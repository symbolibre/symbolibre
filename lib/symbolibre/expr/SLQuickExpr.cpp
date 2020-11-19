// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SLQuickExpr.hpp"

#include <QDebug>
#include <QJsonDocument>

SLQuickExpr::SLQuickExpr(QQuickItem *parent) : QQuickPaintedItem(parent),
    m_expr(), m_font(), m_textColor(Qt::black), m_halign(AlignLeft), m_valign(AlignTop),
    m_cursorBlink(true), m_cursorTimer(this)
{
    m_font.setHintingPreference(QFont::PreferFullHinting);

    connect(this, SIGNAL(activeFocusChanged(bool)), this, SLOT(updateResetCursor()));
    setImplicitHeight(QFontInfo(m_font).pixelSize());
    setImplicitWidth(1);
    setAcceptedMouseButtons(Qt::AllButtons);
    connect(&m_cursorTimer, &QTimer::timeout, [&]() {
        m_cursorBlink = !m_cursorBlink;
        update();
    });
    m_cursorTimer.start(500);
    // QTextLayout forces QPainter::RasterOp_NotDestination composition mode,
    // which does not behave correctly when the background is transparent
    setFillColor(Qt::white);
    connect(this, &SLQuickExpr::fillColorChanged, [&]() {
        update();
        if (fillColor().alpha() != 255)
            qWarning() << "transparent fill colors are not supported by SLQuickExpr";
    });
    setOpaquePainting(true);
}

QString SLQuickExpr::json() const
{
    QJsonDocument doc;
    doc.setArray(m_expr.serialize(true));
    return doc.toJson(QJsonDocument::Compact);
}

void SLQuickExpr::setJson(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray())
        qDebug() << "bad editiontree json";
    m_expr.deserialize(doc.array());
    emit exprChanged();
    emit cursorPosChanged();
    updateResetCursor();
}

void SLQuickExpr::insertJson(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isArray())
        m_expr.insert(new Flow(deserializeFlow(doc.array())));

    else if (doc.isObject()) {
        auto node = deserializeInternalNode(doc.object());
        if (node)
            m_expr.insert(node);
    }

    else {
        qDebug() << "bad editionnode json";
        return;
    }

    emit exprChanged();
    emit cursorPosChanged();
    updateResetCursor();
}

bool SLQuickExpr::dropCursorLeft()
{
    updateResetCursor();
    bool ret = m_expr.dropCursor(MLEFT);
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::dropCursorRight()
{
    updateResetCursor();
    bool ret = m_expr.dropCursor(MRIGHT);
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorLeft()
{
    updateResetCursor();
    bool ret = m_expr.editMoveLeft();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorRight()
{
    updateResetCursor();
    bool ret = m_expr.editMoveRight();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorUp()
{
    updateResetCursor();
    bool ret = m_expr.editMoveUp();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorDown()
{
    updateResetCursor();
    bool ret = m_expr.editMoveDown();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::deleteChar()
{
    updateResetCursor();
    bool ret = m_expr.editDelete();
    if (ret) {
        emit exprChanged();
        emit cursorPosChanged();
    }
    return ret;
}

bool SLQuickExpr::clear()
{
    updateResetCursor();
    bool ret = m_expr.editClear();
    emit exprChanged();
    emit cursorPosChanged();
    return ret;
}

void SLQuickExpr::setFont(const QFont &font)
{
    if (font == m_font)
        return;

    m_font = font;
    updateResetCursor();
    emit fontChanged(m_font);
}

void SLQuickExpr::setColor(QColor color)
{
    if (color == m_textColor)
        return;

    m_textColor = color;
    updateResetCursor();
    emit colorChanged(color);
}

void SLQuickExpr::setHorizontalAlignment(HorizontalAlignment align)
{
    if (align == m_halign)
        return;

    m_halign = align;
    update();
}

void SLQuickExpr::setVerticalAlignment(VerticalAlignment align)
{
    if (align == m_valign)
        return;

    m_valign = align;
    update();
}

void SLQuickExpr::updateResetCursor()
{
    update();
    m_cursorBlink = true;
    if (hasActiveFocus())
        m_cursorTimer.start();
    else
        m_cursorTimer.stop();
}

void SLQuickExpr::paint(QPainter *painter)
{
    painter->setFont(m_font);
    painter->setPen(m_textColor);

    m_expr.computeDimensions(*painter);
    setImplicitWidth(m_expr.getWidth());
    setImplicitHeight(m_expr.getHeight());

    // We don't want to display a box in an entirely empty expression
    if (m_expr.empty() && (!hasActiveFocus() || !m_cursorBlink))
        return;

    QPoint p = m_expr.getCursorCoordinates();

    // expr.draw() updates the position of the cursor
    // but we need to know the position of the cursor before drawing
    // in order to set the viewport if the expression does not fit completely
    // so we draw the expression twice as a workaround
    // FIXME we could compute node relative positions in computeDimensions()
    if (m_expr.getWidth() > width() || m_expr.getHeight() > height()) {
        m_expr.draw(0, 0, *painter, hasActiveFocus() && m_cursorBlink);
        painter->eraseRect(painter->viewport());
        p = m_expr.getCursorCoordinates();
    }

    int x, y;
    if (m_expr.getWidth() <= width()) {
        if (m_halign == AlignLeft)
            x = 0;
        else if (m_halign == AlignRight)
            x = width() - m_expr.getWidth();
        else
            x = width() / 2 - m_expr.getWidth() / 2;
    } else {
        x = width() / 2 - p.x();
    }

    if (m_expr.getHeight() <= height()) {
        if (m_valign == AlignTop)
            y = 0;
        else if (m_valign == AlignBottom)
            y = height() - m_expr.getHeight();
        else
            y = height() / 2 - m_expr.getHeight() / 2;
    } else {
        y = height() / 2 - p.y();
    }

    painter->translate(x, y);
    m_expr.draw(0, 0, *painter, hasActiveFocus() && m_cursorBlink);
}

void SLQuickExpr::keyPressEvent(QKeyEvent *event)
{
    bool accepted = false;
    switch (event->key()) {

    case Qt::Key_Left:
        accepted = moveCursorLeft();
        break;

    case Qt::Key_Up:
        accepted = moveCursorUp();
        break;

    case Qt::Key_Right:
        accepted = moveCursorRight();
        break;

    case Qt::Key_Down:
        accepted = moveCursorDown();
        break;
    }
    if (!accepted)
        event->ignore();
}

void SLQuickExpr::mousePressEvent(QMouseEvent * /*event*/)
{
    forceActiveFocus();
}
