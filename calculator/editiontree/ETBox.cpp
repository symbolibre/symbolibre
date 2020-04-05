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
    doc.setArray(expr.serialize());
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

bool ETBox::recvInput(int /* KeyCode::keycode */ input)
{
    bool ret = true;

    if (KeyCode::SLK_A <= input && input <= KeyCode::SLK_Z)
        expr.editChar('A' + input - KeyCode::SLK_A);
    else if (KeyCode::SLK_a <= input && input <= KeyCode::SLK_z)
        expr.editChar('a' + input - KeyCode::SLK_a);
    else if (KeyCode::SLK_0 <= input && input <= KeyCode::SLK_9)
        expr.editChar('0' + input - KeyCode::SLK_0);
    else
        switch (input) {
        /* Mathematical Operands */
        case KeyCode::SLK_PLUS:
            expr.editOperator('+', QString("+"));
            break;
        case KeyCode::SLK_MINUS:
            expr.editOperator('-', QString("-"));
            break;
        case KeyCode::SLK_TIMES:
            expr.editOperator('*', QString("×"));
            break;
        case KeyCode::SLK_DIVIDE:
            expr.editFrac();
            break;
        case KeyCode::SLK_LPAREN:
            expr.editParen(LPAREN);
            break;
        case KeyCode::SLK_RPAREN:
            expr.editParen(RPAREN);
            break;
        case KeyCode::SLK_SQUAREROOT:
            expr.editRoot();
            break;
        case KeyCode::SLK_POWER:
            expr.editPower();
            break;
        case KeyCode::SLK_SIGMA:
            expr.editSigma();
            break;

        /* Arrows */
        case KeyCode::SLK_UP:
            ret = expr.editMoveUp();
            break;

        case KeyCode::SLK_DOWN:
            ret = expr.editMoveDown();
            break;

        case KeyCode::SLK_RIGHT:
            ret = expr.editMoveRight();
            break;

        case KeyCode::SLK_LEFT:
            ret = expr.editMoveLeft();
            break;

        /* Special keys */
        case KeyCode::SLK_DEL:
            expr.editDelete();
            break;
        case KeyCode::SLK_CLEAR:
            expr.editClear();
            break;

        case KeyCode::SLK_COLONEQ:
            expr.editChar(':');
            expr.editChar('=');
            break;

        case KeyCode::SLK_EXE:
            // What do I do ?
            break;

        default:
            std::cerr << "Unsupported key " << input << std::endl;
            break;
        }

    // expr.ascii(true);
    emit exprChanged();
    update();
    return ret;
}
