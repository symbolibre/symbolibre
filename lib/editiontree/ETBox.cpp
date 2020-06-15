#include "EditionTree.hpp"

#include <QDebug>
#include <QJsonDocument>

ETBox::ETBox(QQuickItem *parent) : QQuickPaintedItem(parent),
    expr(), textColor(Qt::black), halign(AlignLeft), valign(AlignTop),
    cursorBlink(true), cursorTimer(this)
{
    connect(this, SIGNAL(activeFocusChanged(bool)), this, SLOT(updateResetCursor()));
    setImplicitHeight(FONT_SIZE);
    setImplicitWidth(1);
    connect(&cursorTimer, &QTimer::timeout, [&]() {
        cursorBlink = !cursorBlink;
        update();
    });
    cursorTimer.start(500);
    // QTextLayout forces QPainter::RasterOp_NotDestination composition mode,
    // which does not behave correctly when the background is transparent
    setFillColor(Qt::white);
    connect(this, &ETBox::fillColorChanged, [&]() {
        update();
        if (fillColor().alpha() != 255)
            qWarning() << "transparent fill colors are not supported by ETBox";
    });
    setOpaquePainting(true);
}

QString ETBox::json() const
{
    QJsonDocument doc;
    doc.setArray(expr.serialize(true));
    return doc.toJson(QJsonDocument::Compact);
}

void ETBox::setJson(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray())
        qDebug() << "bad editiontree json";
    expr.deserialize(doc.array());
    emit exprChanged();
    emit cursorPosChanged();
    updateResetCursor();
}

void ETBox::insertJson(const QString &json)
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

bool ETBox::moveCursorLeft()
{
    updateResetCursor();
    bool ret = expr.editMoveLeft();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool ETBox::moveCursorRight()
{
    updateResetCursor();
    bool ret = expr.editMoveRight();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool ETBox::moveCursorUp()
{
    updateResetCursor();
    bool ret = expr.editMoveUp();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool ETBox::moveCursorDown()
{
    updateResetCursor();
    bool ret = expr.editMoveDown();
    if (ret)
        emit cursorPosChanged();
    return ret;
}

bool ETBox::deleteChar()
{
    updateResetCursor();
    bool ret = expr.editDelete();
    if (ret) {
        emit exprChanged();
        emit cursorPosChanged();
    }
    return ret;
}

bool ETBox::clear()
{
    updateResetCursor();
    bool ret = expr.editClear();
    emit exprChanged();
    emit cursorPosChanged();
    return ret;
}

void ETBox::setColor(QColor color)
{
    textColor = color;
    update();
    updateResetCursor();
    emit colorChanged(color);
}

void ETBox::setHorizontalAlignment(HorizontalAlignment align)
{
    halign = align;
    update();
}

void ETBox::setVerticalAlignment(VerticalAlignment align)
{
    valign = align;
    update();
}

void ETBox::updateResetCursor()
{
    update();
    cursorBlink = true;
    if (hasActiveFocus())
        cursorTimer.start();
    else
        cursorTimer.stop();
}

void ETBox::paint(QPainter *painter)
{
    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(FONT_SIZE);
    painter->setFont(font);
    painter->setPen(textColor);

    expr.computeDimensions(*painter);
    setImplicitWidth(expr.getWidth());
    setImplicitHeight(expr.getHeight());

    // We don't want to display a box in an entirely empty expression
    if (expr.empty() && (!hasActiveFocus() || !cursorBlink))
        return;

    QPoint p = expr.getCursorCoordinates();
    int x, y;
    if (expr.getWidth() <= width()) {
        if (halign == AlignLeft)
            x = 0;
        else if (halign == AlignRight)
            x = width() - expr.getWidth();
        else
            x = width() / 2 - expr.getWidth() / 2;
    } else
        x = - (expr.getWidth() - p.x());

    if (expr.getHeight() <= height()) {
        if (valign == AlignTop)
            y = 0;
        else if (valign == AlignBottom)
            y = height() - expr.getHeight();
        else
            y = height() / 2 - expr.getHeight() / 2;
    } else if (expr.getHeight() - expr.getCenterHeight() < (int) height() / 2)
        y = height() / 2 + expr.getCenterHeight() - expr.getHeight();
    else
        y = height() / 2 + expr.getCenterHeight() - expr.getHeight() - p.y();

    expr.draw(x, y, *painter, hasActiveFocus() && cursorBlink);
}

