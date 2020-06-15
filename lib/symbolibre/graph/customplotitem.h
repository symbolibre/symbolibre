#ifndef CUSTOMPLOTITEM_H
#define CUSTOMPLOTITEM_H

#include <QtQuick>
#include "../keyboard/keycode.hpp"
#include "curveitem.h"

class QCustomPlot;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QRectF view READ view WRITE setRange NOTIFY viewChanged)
    Q_PROPERTY(QPointF cursorPos READ cursorPos NOTIFY cursorPosChanged)
    Q_PROPERTY(QString selectedCurve READ selectedCurve WRITE setSelectedCurve NOTIFY selectedCurveChanged)

public:
    CustomPlotItem(QQuickItem *parent = nullptr);
    virtual ~CustomPlotItem();

    void paint(QPainter *painter);
    void plotGraph(QString nomGraph);

    QString selectedCurve() const;
    const QRectF &view() const;
    QPointF cursorPos() const;

    static void declareQML()
    {
        qmlRegisterType<CustomPlotItem>("SLCustomPlotItem", 1, 0, "CustomPlotItem");
    }

public slots:
    void setSelectedCurve(QString curve);

signals:
    void viewChanged(QRectF);
    void cursorPosChanged(QPointF);
    void selectedCurveChanged(QString);

private:
    QCustomPlot m_CustomPlot;

    QRectF m_view;
    QPointF m_cursorPos;

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
    QMap<QString, CurveItem> listGraph;
    int nbCurves;

public:
    Q_INVOKABLE void recvInput(int input);
    Q_INVOKABLE void addGraph(QString formula, QColor color = Qt::black);
    Q_INVOKABLE void setRange(const QRectF &range);
    Q_INVOKABLE void moveWindow(QPoint offset);
    Q_INVOKABLE void moveWindow(int x, int y);
    Q_INVOKABLE void moveCursor(int amtX, int amtY);
    Q_INVOKABLE void modifyZoom(double value);
    Q_INVOKABLE void setModeWindow();
    Q_INVOKABLE void setModeCursor();
    Q_INVOKABLE void switchModeCurWin();

private slots:
    void updateCustomPlotSize();
    void onCustomReplot();
    void clearGraph();
    void removeGraph(QString nomGraph);
};

#endif // CUSTOMPLOTITEM_H
