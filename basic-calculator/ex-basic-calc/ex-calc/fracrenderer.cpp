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

void FracRenderer::recvARROW(arrowkey dir)
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

void FracRenderer::recvDIGIT(int digit)
{
    expr.editDigit(digit);
    update();
}

void FracRenderer::recvDELETE(void)
{
    expr.editDelete();
    update();
}

void FracRenderer::paintEvent(QPaintEvent * /* */)
{
    /* Doing nothing ! */
    std::cout << "NEW EDIT" << std::endl;
    expr.ascii(0, true);
}


