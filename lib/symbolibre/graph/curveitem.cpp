#include "curveitem.h"

#include "symbolibre/cas/MathContext.hpp"

CurveItem::CurveItem(QString name, QCPGraph *graphustule, QColor color) :
    name(name), graph(graphustule)
{
    graph->setPen(QPen(color));
}

CurveItem::~CurveItem()
{

}

void CurveItem::clear()
{
    graph->data()->clear();
}

double CurveItem::getValue(double x, MathContext *ctx)
{
    giac::gen y = ctx->giacEvalString(name + "(" + QString::number(x) + ")");

    //if (y.is_real(ctx->giacContext()))
    return y.to_double(ctx->giacContext()); // returns NaN if y is not real
}

void CurveItem::setColor(QColor color)
{
    graph->setPen(QPen(color));
}
