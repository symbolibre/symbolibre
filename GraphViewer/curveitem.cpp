#include "curveitem.h"

CurveItem::CurveItem(std::string form)
{
    giac::context ct;
    formula = form;
    f = eval(giac::gen("x->" + form, &ct), &ct);
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
