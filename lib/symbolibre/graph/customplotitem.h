#ifndef CUSTOMPLOTITEM_H
#define CUSTOMPLOTITEM_H

#include <QQuickPaintedItem>
#include "curveitem.h"

class QCustomPlot;
class MathContext;

class CustomPlotItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(MathContext *mathContext READ mathContext WRITE setMathContext NOTIFY mathContextChanged)
    Q_PROPERTY(QRectF view READ view WRITE setRange NOTIFY viewChanged)
    Q_PROPERTY(QPointF cursorPos READ cursorPos NOTIFY cursorPosChanged)
    Q_PROPERTY(QString selectedCurve READ selectedCurve WRITE setSelectedCurve NOTIFY selectedCurveChanged)
    Q_PROPERTY(bool cursorAttached
        READ isCursorAttached WRITE setCursorAttached NOTIFY cursorAttachedChanged)

public:
    CustomPlotItem(QQuickItem *parent = nullptr);

    void paint(QPainter *painter);
    void plotGraph(QString nomGraph);
    void replotGraphs();

    MathContext * mathContext() const;
    QString selectedCurve() const;
    const QRectF &view() const;
    QPointF cursorPos() const;
    bool isCursorAttached() const;

    double xScale() const;
    double yScale() const;

public slots:
    void setMathContext(MathContext *ctx);
    void setSelectedCurve(QString curve);
    void setCursorAttached(bool attached);

signals:
    void mathContextChanged(MathContext *);
    void viewChanged(QRectF);
    void cursorPosChanged(QPointF);
    void selectedCurveChanged(QString);
    void cursorAttachedChanged(bool);

private:
    MathContext *mMathContext;

    QCustomPlot m_CustomPlot;

    QRectF m_view;
    bool mCursorAttached;

    /* Position of the cursor */
    QCPItemTracer *cursor;

    /* curves on the graph */
    QMap<QString, CurveItem> listGraph;

public:
    Q_INVOKABLE void addGraph(QString formula, QColor color = Qt::black);
    Q_INVOKABLE void setRange(const QRectF &range);
    Q_INVOKABLE void setRange(qreal xmin, qreal xmax, qreal ymin, qreal ymax);
    Q_INVOKABLE void moveWindow(QPoint offset);
    Q_INVOKABLE void moveWindow(int x, int y);
    Q_INVOKABLE void moveCursor(int amtX, int amtY);
    Q_INVOKABLE void zoomIn(double value);

public slots:
    void clearGraph();

private slots:
    void updateCustomPlotSize();
    void removeGraph(QString nomGraph);

private:
    void redraw();
};

#endif // CUSTOMPLOTITEM_H
