#include "fracrenderer.h"
#include "flow.h"

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
    return QSize(400, 300);
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

void FracRenderer::recvPlus()
{
    expr.editChar('+');
    update();
}

void FracRenderer::recvMinus()
{
    expr.editChar('-');
    update();
}

void FracRenderer::recvTimes()
{
    expr.editChar('*');
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
    /* Doing nothing ! */
    std::cout << '[' << expr.get_text() << ']' << std::endl;
    expr.ascii(0, true);

    QFont font = QFont("dejavu");
    font.setStyleHint(QFont::Monospace);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setPixelSize(20);
    QPainter painter(this);
    painter.setFont(font);

    expr.compute_dimensions(painter);
    expr.draw(0, 0, painter, true);
}


