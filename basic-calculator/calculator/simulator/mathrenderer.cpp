#include "mathrenderer.hpp"
#include <iostream>
#include <QFont>

MathRenderer::MathRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    expression = Flow(ROOT);
    expression.editChar('w');
    expression.editChar('o');
    expression.editChar('r');
    expression.editChar('k');
    expression.editChar('i');
    expression.editChar('n');
    expression.editChar('g');
}

void MathRenderer::paint(QPainter *painter)
{
    //expression.ascii(0, true);

    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(MIN_SIZE);
    painter->setFont(font);

    expression.computeDimensions(*painter);
    expression.draw(0, 0, *painter, true);
    //painter->drawRect(0,0, 50, 50);
    return;
}

void MathRenderer::recvInput(int /* KeyCode::keycode */ input)
{
    // std::cerr << "Received INPUT code: " << input << std::endl;

    switch (input) {
    /* Digits */
    case KeyCode::SLK_0:
        expression.editChar('0');
        break;
    case KeyCode::SLK_1:
        expression.editChar('1');
        break;
    case KeyCode::SLK_2:
        expression.editChar('2');
        break;
    case KeyCode::SLK_3:
        expression.editChar('3');
        break;
    case KeyCode::SLK_4:
        expression.editChar('4');
        break;
    case KeyCode::SLK_5:
        expression.editChar('5');
        break;
    case KeyCode::SLK_6:
        expression.editChar('6');
        break;
    case KeyCode::SLK_7:
        expression.editChar('7');
        break;
    case KeyCode::SLK_8:
        expression.editChar('8');
        break;
    case KeyCode::SLK_9:
        expression.editChar('9');
        break;

    /* Mathematical Operands */
    case KeyCode::SLK_PLUS:
        expression.editChar('+');
        break;
    case KeyCode::SLK_MINUS:
        expression.editChar('-');
        break;
    case KeyCode::SLK_TIMES:
        expression.editChar('*');
        break;
    case KeyCode::SLK_DIVIDE:
        expression.editFrac();
        break;
    case KeyCode::SLK_LPAREN:
        expression.editParen(LPAREN);
        break;
    case KeyCode::SLK_RPAREN:
        expression.editParen(RPAREN);
        break;
    case KeyCode::SLK_SQUAREROOT:
        expression.editRoot();
        break;

    /* Arrows */
    case KeyCode::SLK_UP:
        expression.editMoveUp();
        break;
    case KeyCode::SLK_DOWN:
        expression.editMoveDown();
        break;
    case KeyCode::SLK_RIGHT:
        expression.editMoveRight();
        break;
    case KeyCode::SLK_LEFT:
        expression.editMoveLeft();
        break;

    /* Special keys */
    case KeyCode::SLK_DEL:
        expression.editDelete();
        break;
    case KeyCode::SLK_CLEAR:
        expression.editClear();
        break;

    default:
        std::cerr << "Unsupported key " << input << std::endl;
        break;
    }

    emit expressionChanged();
    update();
}
