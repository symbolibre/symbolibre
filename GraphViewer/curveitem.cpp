#include "curveitem.h"

CurveItem::CurveItem(std::string form)
{
    formula = form;
}

double CurveItem::getValue(double x)
{
    std::ostringstream sstream;
    sstream << x;
    std::string ant = sstream.str();
    std::size_t found = ant.find('.');
    if (found == std::string::npos) {
        ant += ".0";
    }

    double y = approximate(formula, ant);
    return y;
}
