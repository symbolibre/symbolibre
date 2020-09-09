#include "SLQuickExpr.hpp"

#include <QDebug>
#include <QJsonDocument>

SLQuickExpr::SLQuickExpr(QQuickItem *parent) : QQuickPaintedItem(parent),
    expr(), mFont(), textColor(Qt::black), halign(AlignLeft), valign(AlignTop),
    cursorBlink(true), cursorTimer(this)
{
    mFont.setHintingPreference(QFont::PreferFullHinting);

    connect(this, SIGNAL(activeFocusChanged(bool)), this, SLOT(updateResetCursor()));
    setImplicitHeight(QFontInfo(mFont).pixelSize());
    setImplicitWidth(1);
    setAcceptedMouseButtons(Qt::AllButtons);
    connect(&cursorTimer, &QTimer::timeout, [&]() {
        cursorBlink = !cursorBlink;
        update();
    });
    cursorTimer.start(500);
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
    doc.setArray(expr.serialize(true));
    return doc.toJson(QJsonDocument::Compact);
}

void SLQuickExpr::setJson(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray())
        qDebug() << "bad editiontree json";
    expr.deserialize(doc.array());
    emit exprChanged();
    emit cursorPosChanged();
    updateResetCursor();
}

void SLQuickExpr::insertJson(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isArray())
        expr.insert(new Flow(deserializeFlow(doc.array())));

    else if (doc.isObject()) {
        auto node = deserializeInternalNode(doc.object());
        if (node)
            expr.insert(node);
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
    bool ret = expr.dropCursor(MLEFT);
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::dropCursorRight()
{
    updateResetCursor();
    bool ret = expr.dropCursor(MRIGHT);
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorLeft()
{
    updateResetCursor();
    bool ret = expr.editMoveLeft();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorRight()
{
    updateResetCursor();
    bool ret = expr.editMoveRight();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorUp()
{
    updateResetCursor();
    bool ret = expr.editMoveUp();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::moveCursorDown()
{
    updateResetCursor();
    bool ret = expr.editMoveDown();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool SLQuickExpr::deleteChar()
{
    updateResetCursor();
    bool ret = expr.editDelete();
    if (ret) {
        emit exprChanged();
        emit cursorPosChanged();
    }
    return ret;
}

bool SLQuickExpr::clear()
{
    updateResetCursor();
    bool ret = expr.editClear();
    emit exprChanged();
    emit cursorPosChanged();
    return ret;
}

void SLQuickExpr::setFont(const QFont &font)
{
    mFont = font;
    updateResetCursor();
    emit fontChanged(mFont);
}

void SLQuickExpr::setColor(QColor color)
{
    textColor = color;
    updateResetCursor();
    emit colorChanged(color);
}

void SLQuickExpr::setHorizontalAlignment(HorizontalAlignment align)
{
    halign = align;
    update();
}

void SLQuickExpr::setVerticalAlignment(VerticalAlignment align)
{
    valign = align;
    update();
}

void SLQuickExpr::updateResetCursor()
{
    update();
    cursorBlink = true;
    if (hasActiveFocus())
        cursorTimer.start();
    else
        cursorTimer.stop();
}

void SLQuickExpr::paint(QPainter *painter)
{
    painter->setFont(mFont);
    painter->setPen(textColor);

    expr.computeDimensions(*painter);
    setImplicitWidth(expr.getWidth());
    setImplicitHeight(expr.getHeight());

    // We don't want to display a box in an entirely empty expression
    if (expr.empty() && (!hasActiveFocus() || !cursorBlink))
        return;

    QPoint p = expr.getCursorCoordinates();

    // expr.draw() updates the position of the cursor
    // but we need to know the position of the cursor before drawing
    // in order to set the viewport if the expression does not fit completely
    // so we draw the expression twice as a workaround
    // FIXME we could compute node relative positions in computeDimensions()
    if (expr.getWidth() > width() || expr.getHeight() > height()) {
        expr.draw(0, 0, *painter, hasActiveFocus() && cursorBlink);
        painter->eraseRect(painter->viewport());
        p = expr.getCursorCoordinates();
    }

    int x, y;
    if (expr.getWidth() <= width()) {
        if (halign == AlignLeft)
            x = 0;
        else if (halign == AlignRight)
            x = width() - expr.getWidth();
        else
            x = width() / 2 - expr.getWidth() / 2;
    } else {
        x = width() / 2 - p.x();
    }

    if (expr.getHeight() <= height()) {
        if (valign == AlignTop)
            y = 0;
        else if (valign == AlignBottom)
            y = height() - expr.getHeight();
        else
            y = height() / 2 - expr.getHeight() / 2;
    } else {
        y = height() / 2 - p.y();
    }

    painter->translate(x, y);
    expr.draw(0, 0, *painter, hasActiveFocus() && cursorBlink);
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
