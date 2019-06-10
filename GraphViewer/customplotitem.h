#ifndef CUSTOMPLOTITEM_H
#define CUSTOMPLOTITEM_H

#include <QtQuick>
#include "keycode.hpp"
#include "curveitem.h"

class QCustomPlot;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(double cursorX READ getCursorX NOTIFY cursorXChanged)
    Q_PROPERTY(double cursorY READ getCursorY NOTIFY cursorYChanged)

public:
    CustomPlotItem(QQuickItem *parent = nullptr);
    virtual ~CustomPlotItem();

    void paint(QPainter *painter);
    void plotGraph(QString nomGraph);


    static void declareQML()
    {
        qmlRegisterType<CustomPlotItem>("SLCustomPlotItem", 1, 0, "CustomPlotItem");
    }


signals:
    void cursorXChanged();
    void cursorYChanged();

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
    /* Scale of the axis to ensure smooth plot */
    double Xsca;
    double Ysca;

    /* Position of the cursor */
    QCPItemTracer *cursor;
    int cursorVisible;
    double cursorX;
    double cursorY;
    int modeCursor;

    /* curves on the graph */
    QColor listColor[4] = {Qt::red, Qt::blue, Qt::green, Qt::black};
    QMap<QString, CurveItem> listGraph;
    int nbCurves;

public slots:
    Q_INVOKABLE void recvInput(int input);
    Q_INVOKABLE void addGraph(QString formula);
    Q_INVOKABLE void addGraph(QString formula, QColor color);
    Q_INVOKABLE void setRange(double nXmin, double nXmax, double nYmin, double nYmax);
    Q_INVOKABLE void moveWindow(int horizontal, int vertical);
    Q_INVOKABLE void moveCursor(int amtX, int amtY);
    Q_INVOKABLE void modifyZoom(double value);
    Q_INVOKABLE double getXmin();
    Q_INVOKABLE double getXmax();
    Q_INVOKABLE double getYmin();
    Q_INVOKABLE double getYmax();
    Q_INVOKABLE void setModeWindow();
    Q_INVOKABLE void setModeCursor();
    Q_INVOKABLE void switchModeCurWin();
    Q_INVOKABLE double getCursorX();
    Q_INVOKABLE double getCursorY();



private slots:
    void updateCustomPlotSize();
    void onCustomReplot();
    void clearGraph();
    void removeGraph(QString nomGraph);
};

#endif // CUSTOMPLOTITEM_H
