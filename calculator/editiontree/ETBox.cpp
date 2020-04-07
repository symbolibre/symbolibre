#include "EditionTree.hpp"
#include "keycode.hpp"

#include <QDebug>
#include <QJsonDocument>

ETBox::ETBox(QQuickItem *parent) : QQuickPaintedItem(parent),
    centerOnCursor(0), adjustHeight(0), adjustWidth(0)
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
    if (expr.getWidth() < (int) width())
        x = 0;
    else
        x = - (expr.getWidth() - p.x());

    // center expression vertically if there is enough room
    if (expr.getHeight() <= height())
        y = height() / 2 - expr.getHeight() / 2;
    else if (expr.getHeight() - expr.getCenterHeight() < (int) height() / 2)
        y = height() / 2 + expr.getCenterHeight() - expr.getHeight();
    else
        y = height() / 2 + expr.getCenterHeight() - expr.getHeight() - p.y();

    expr.draw(x, y, *painter, hasActiveFocus());
}

