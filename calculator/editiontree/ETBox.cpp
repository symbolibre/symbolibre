#include "EditionTree.hpp"
#include "keycode.hpp"

#include <QDebug>
#include <QJsonDocument>

ETBox::ETBox(QQuickItem *parent) : QQuickPaintedItem(parent),
    halign(), valign()
{
    expr = EditionTree();
    connect(this, SIGNAL(activeFocusChanged(bool)), this, SLOT(update()));
    setImplicitHeight(FONT_SIZE);
}

QString ETBox::json() const
{
    QJsonDocument doc;
    doc.setArray(expr.serialize(true));
    return doc.toJson();
}

void ETBox::setJson(const QString &json)
{
    auto doc = QJsonDocument::fromJson(json.toUtf8());
    if (!doc.isArray())
        qDebug() << "bad editiontree json";
    expr.deserialize(doc.array());
    update();
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
    update();
}

bool ETBox::moveCursorLeft()
{
    update();
    return expr.editMoveLeft();
}

bool ETBox::moveCursorRight()
{
    update();
    return expr.editMoveRight();
}

bool ETBox::moveCursorUp()
{
    update();
    return expr.editMoveUp();
}

bool ETBox::moveCursorDown()
{
    update();
    return expr.editMoveDown();
}

bool ETBox::deleteChar()
{
    update();
    return expr.editDelete();
}

bool ETBox::clear()
{
    update();
    return expr.editClear();
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

void ETBox::paint(QPainter *painter)
{
    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(FONT_SIZE);
    painter->setFont(font);

    expr.computeDimensions(*painter);
    setImplicitWidth(expr.getWidth());
    setImplicitHeight(expr.getHeight());

    QPoint p = expr.getCursorCoordinates();
    int x, y;
    if (expr.getWidth() < width()) {
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

    expr.draw(x, y, *painter, hasActiveFocus());
}

