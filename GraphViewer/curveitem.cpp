#include "curveitem.h"

CurveItem::CurveItem(std::string form, QCPGraph *graphustule, QColor couleur)
{
    giac::context ct;
    formula = form;
    f = eval(giac::gen("x->" + form, &ct), &ct);

    graph = graphustule;
    color = couleur;
    graph->setPen(QPen(couleur));
}

CurveItem::~CurveItem()
{
    //delete f;
    //delete graph;
}

double CurveItem::getValue(double x)
{
    giac::context ct;
    giac::gen y = f(x, &ct);
    double result;

    if (y.type == giac::_DOUBLE_) {
        result = y._DOUBLE_val;
    } else {
        result = (double) y.val;
    }
    return result;
}

void CurveItem::updateFormula(std::string form)
{
    giac::context ct;
    formula = form;
    f = eval(giac::gen("x->" + form, &ct), &ct);

    graph->data()->clear();
}
