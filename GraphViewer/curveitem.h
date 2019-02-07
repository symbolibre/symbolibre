#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <QtQuick>
#include <string>
#include <iostream>
#include "interface.h"

class CurveItem
{
public:
    CurveItem(std::string form);
    double getValue(double x);

private:
    std::string formula;
};

#endif // CURVEITEM_H
