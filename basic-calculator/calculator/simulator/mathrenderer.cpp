#include "mathrenderer.hpp"
#include <iostream>
#include <QFont>
#include <string>

MathRenderer::MathRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    expression = Flow(ROOT);
}

void MathRenderer::paint(QPainter *painter)
{
    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(MIN_SIZE);
    painter->setFont(font);

    expression.computeDimensions(*painter);
    expression.draw(0, 0, *painter, true);
    return;
}

void MathRenderer::recvInput(int /* KeyCode::keycode */ input)
{
    if (KeyCode::SLK_A <= input && input <= KeyCode::SLK_Z)
        expression.editChar('A' + input - KeyCode::SLK_A);
    else if (KeyCode::SLK_0 <= input && input <= KeyCode::SLK_9)
        expression.editChar('0' + input - KeyCode::SLK_0);
    else
        switch (input) {
        /* Mathematical Operands */
        case KeyCode::SLK_PLUS:
            expression.editOperator('+', QString("+"));
            break;
        case KeyCode::SLK_MINUS:
            expression.editOperator('-', QString("-"));
            break;
        case KeyCode::SLK_TIMES:
            expression.editOperator('*', QString("×"));
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

        case KeyCode::SLK_EXE:
            std::cout << expression.getText() << " = " << evaluate(expression.getText()) << std::endl; // TODO no copy
            expression.editClear();
            break;

        default:
            std::cerr << "Unsupported key " << input << std::endl;
            break;
        }

    emit expressionChanged();
    update();
}
