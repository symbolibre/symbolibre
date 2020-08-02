#ifndef CURVEITEM_H
#define CURVEITEM_H

#include <qcustomplot.h>
#include <giac/giac.h>

class MathContext;

class CurveItem
{
public:
    CurveItem() = default;
    CurveItem(QString name, QCPGraph *graphustule, QColor color);

    void clear();
    double getValue(double x, MathContext *ctx);
    void setColor(QColor color);
    QCPGraph *graph;

private:
    QString name;
    giac::gen f;
    QColor color;
};

#endif // CURVEITEM_H
