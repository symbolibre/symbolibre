#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <QtQuick>
#include <string>
#include <iostream>
#include <giac/config.h>
#include <giac/giac.h>
#include "qcustomplot.h"

class CurveItem
{
public:
    CurveItem() = default;
    CurveItem(std::string form, QCPGraph *graphustule, QColor couleur);
    virtual ~CurveItem();

    double getValue(double x);
    void updateFormula(std::string form);
    QCPGraph *graph;

private:
    std::string formula;
    giac::gen f;
    QColor color;
};

#endif // CURVEITEM_H
