#ifndef CUSTOMPLOTITEM_H
#define CUSTOMPLOTITEM_H

#include <QtQuick>
#include "keycode.hpp"
#include "curveitem.h"

class QCustomPlot;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    CustomPlotItem(QQuickItem *parent = nullptr);
    virtual ~CustomPlotItem();

    void paint(QPainter *painter);
    void plotGraph(int numGraph);
    void setRange(double nXmin, double nXmax, double nYmin, double nYmax);
    void moveWindow(int horizontal, int vertical);
    void moveCursor(int amtX, int amtY);
    void modifyZoom(double value);

private:
    QCustomPlot *m_CustomPlot;

    /* Upper and lower bound of the screen */
    double Xmin;
    double Xmax;
    double Ymin;
    double Ymax;

    /* Center of the screen */
    double Xcen;
    double Ycen;
    double Xlen;
    double Ylen;
    /* Scale of X Axis to ensure smooth plot */
    double Xsca;
    double Ysca;

    /* Position of the cursor */
    double cursorX;
    double cursorY;

    /* curves on the graph */
    QList<CurveItem> listGraph;
    int nbCurves;

public slots:
    Q_INVOKABLE void recvInput(int input);
    Q_INVOKABLE void addGraph(QString formula);

private slots:
    void updateCustomPlotSize();
    void onCustomReplot();
    void clearGraph();
    void removeGraph(int numGraph);
};

#endif // CUSTOMPLOTITEM_H
