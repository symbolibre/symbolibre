#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <QtQuick>
#include <string>
#include <iostream>
#include <giac/config.h>
#include <giac/giac.h>
//#include "interface.h"

class CurveItem
{
public:
    CurveItem(std::string form);
    double getValue(double x);

private:
    std::string formula;
    giac::gen f;
};

#endif // CURVEITEM_H
