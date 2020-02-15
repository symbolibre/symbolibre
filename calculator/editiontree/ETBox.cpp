#include "EditionTree.hpp"
#include "keycode.hpp"

ETBox::ETBox(QQuickItem *parent) : QQuickPaintedItem(parent),
    centerOnCursor(0), adjustHeight(0), adjustWidth(0)
{
    expr = EditionTree();
    connect(this, SIGNAL(activeFocusChanged(bool)), this, SLOT(update()));
}

void ETBox::paint(QPainter *painter)
{
    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(FONT_SIZE);
    painter->setFont(font);

    expr.computeDimensions(*painter);
    QPoint p = expr.getCursorCoordinates();
    const int spaceFromBorder = 5;
    int x, y;
    if (expr.getWidth() < (int) width() - spaceFromBorder)
        x = spaceFromBorder;
    else
        x = spaceFromBorder - (expr.getWidth() - p.x());
    if (expr.getHeight() - expr.getCenterHeight() < (int) height() / 2)
        y = height() / 2 + expr.getCenterHeight() - expr.getHeight();
    else
        y = height() / 2 + expr.getCenterHeight() - expr.getHeight() - p.y();
    painter->drawRect(0, 0, width() - 1, height() - 1);
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
    emit expressionChanged();
    update();
    return ret;
}
