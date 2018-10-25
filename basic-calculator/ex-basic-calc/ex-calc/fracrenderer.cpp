#include "fracrenderer.hpp"
#include "Flow.hpp"

#include <QPainter>

#include <stdlib.h>
#include <algorithm>

FracRenderer::FracRenderer(QWidget *parent)
    : QWidget(parent), expr(ROOT)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

/* About size */
QSize FracRenderer::minimumSizeHint() const
{
    return QSize(200, 100);
}

QSize FracRenderer::sizeHint() const
{
    return QSize(320, 240);
}

void FracRenderer::recvArrow(arrowkey dir)
{
    switch (dir) {
    case LEFT:
        expr.editMoveLeft();
        break;
    case RIGHT:
        expr.editMoveRight();
        break;
    case UP:
        expr.editMoveUp();
        break;
    case DOWN:
        expr.editMoveDown();
        break;
    default:
        break;
    }
    update();
    // TODO
}

void FracRenderer::recvDigit(int digit)
{
    expr.editChar(digit + '0');
    update();
}

void FracRenderer::recvChar(char c)
{
    expr.editChar(c);
    update();
}

void FracRenderer::recvLParen()
{
    expr.editParen(LPAREN);
    update();
}

void FracRenderer::recvRParen()
{
    expr.editParen(RPAREN);
    update();
}

void FracRenderer::recvFrac()
{
    expr.editFrac();
    update();
}

void FracRenderer::recvDelete(void)
{
    expr.editDelete();
    update();
}

void FracRenderer::recvClear()
{
    expr.editClear();
    update();
}

void FracRenderer::paintEvent(QPaintEvent * /* */)
{
    std::cout << expr.getText() << std::endl;

    QFont font = QFont("dejavu sans mono");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(MIN_SIZE);
    QPainter painter(this);
    painter.setFont(font);

    expr.computeDimensions(painter);
    expr.draw(0, 0, painter, true);
}


