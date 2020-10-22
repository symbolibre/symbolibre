#ifndef SLQUICKGRAPH_HPP
#define SLQUICKGRAPH_HPP

#include <QQuickPaintedItem>
#include <qcustomplot.h>

class MathContext;

class SLQuickGraph : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(MathContext *mathContext READ mathContext WRITE setMathContext NOTIFY mathContextChanged)
    Q_PROPERTY(QRectF view READ view WRITE setRange NOTIFY viewChanged)
    Q_PROPERTY(QPointF cursorPos READ cursorPos NOTIFY cursorPosChanged)
    Q_PROPERTY(QString selectedCurve READ selectedCurve WRITE setSelectedCurve NOTIFY selectedCurveChanged)
    Q_PROPERTY(bool cursorAttached
        READ isCursorAttached WRITE setCursorAttached NOTIFY cursorAttachedChanged)

    // Also has public methods addError() and clearErrors()
    Q_PROPERTY(QStringList errors READ errors NOTIFY errorsChanged)

public:
    SLQuickGraph(QQuickItem *parent = nullptr);

    void paint(QPainter *painter);
    void plotGraph(QString nomGraph);
    void replotGraphs();

    MathContext * mathContext() const;
    QString selectedCurve() const;
    const QRectF &view() const;
    QPointF cursorPos() const;
    bool isCursorAttached() const;
    const QStringList &errors() const;

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
    void errorsChanged(const QStringList &);

private:
    MathContext *mMathContext;

    QCustomPlot mPlot;
    QMap<QString, QCPGraph*> mGraphs;
    QCPItemTracer *mCursor;

    QRectF mView;
    bool mCursorAttached;

    QStringList mErrors;


public:
    Q_INVOKABLE void addGraph(QString formula, QColor color = Qt::black);
    Q_INVOKABLE void setRange(const QRectF &range);
    Q_INVOKABLE void setRange(qreal xmin, qreal xmax, qreal ymin, qreal ymax);
    Q_INVOKABLE void moveWindow(QPoint offset);
    Q_INVOKABLE void moveWindow(int x, int y);
    Q_INVOKABLE void moveCursor(int amtX, int amtY);
    Q_INVOKABLE void zoomIn(double value);
    Q_INVOKABLE void addError(QString msg);
    Q_INVOKABLE void clearErrors();

public slots:
    void clearGraph();

private slots:
    void updateCustomPlotSize();
    void removeGraph(QString nomGraph);

private:
    void redraw();
    double getValue(const QString &f, double x, bool *err = nullptr);
};

#endif // SLQUICKGRAPH_HPP
