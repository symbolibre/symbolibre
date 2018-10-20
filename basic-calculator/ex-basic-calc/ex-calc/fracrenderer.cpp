#include "fracrenderer.h"
#include "editiontree.h"

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
        expr.editMOVELEFT();
        break;
    case RIGHT:
        expr.editMOVERIGHT();
        break;
    case UP:
        expr.editMOVEUP();
        break;
    case DOWN:
        expr.editMOVEDOWN();
        break;
    default:
        break;
    }
    update();
    // TODO
}

void FracRenderer::recvDIGIT(int digit)
{
    expr.editDIGIT(digit);
    update();
}

void FracRenderer::recvDELETE(void)
{
    expr.editDELETE();
    update();
}

void FracRenderer::paintEvent(QPaintEvent * /* */)
{
    /* Doing nothing ! */
    std::cout << "NEW EDIT" << std::endl;
    expr.ascii(0, true);
}


