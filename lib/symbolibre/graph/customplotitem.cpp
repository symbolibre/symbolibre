#include "customplotitem.h"

#include <limits>
#include <qcustomplot.h>
#include "symbolibre/cas/MathContext.hpp"

CustomPlotItem::CustomPlotItem(QQuickItem *parent) : QQuickPaintedItem(parent),
    mMathContext(nullptr), mPlot(), mGraphs(), mCursor(new QCPItemTracer(&mPlot)),
    mView(-10, -10, 20, 20), mCursorAttached(false)
{
    mCursor->setStyle(QCPItemTracer::tsPlus);
    mCursor->setVisible(false);

    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize);

    updateCustomPlotSize();
    mPlot.xAxis->setRange(mView.left(), mView.right());
    mPlot.yAxis->setRange(mView.top(), mView.bottom());
    connect(&mPlot, SIGNAL(afterReplot()), this, SLOT(update()));
    redraw();
}

void CustomPlotItem::paint(QPainter *painter)
{
    QPixmap    picture(boundingRect().size().toSize());
    QCPPainter qcpPainter(&picture);
    mPlot.toPainter(&qcpPainter);
    painter->drawPixmap(QPoint(), picture);
}

void CustomPlotItem::plotGraph(QString f)
{
    /* take the name of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    auto g = mGraphs[f];
    g->data()->clear();

    double x = mView.left();
    g->addData(x, getValue(f, x));
    while (x <= mView.right()) {
        x += xScale();
        g->addData(x, getValue(f, x));
    }

    redraw();
}

void CustomPlotItem::replotGraphs()
{
    for (const QString &f : mGraphs.keys())
        plotGraph(f);
}

MathContext * CustomPlotItem::mathContext() const
{
    return mMathContext;
}

void CustomPlotItem::setMathContext(MathContext *ctx)
{
    if (ctx == mMathContext)
        return;

    mMathContext = ctx;
    replotGraphs();
    emit mathContextChanged(ctx);
}

void CustomPlotItem::setRange(const QRectF &range)
{
    if (range == mView)
        return;

    mView = range;
    mPlot.xAxis->setRange(mView.left(), mView.right());
    mPlot.yAxis->setRange(mView.top(), mView.bottom());

    replotGraphs();
    emit viewChanged(mView);
}

void CustomPlotItem::setRange(qreal xmin, qreal xmax, qreal ymin, qreal ymax)
{
    setRange(QRectF(xmin, ymin, xmax - xmin, ymax - ymin));
}

void CustomPlotItem::moveWindow(QPoint offset)
{
    //move the window, keep ratio of size, just add an offset
    if (offset.isNull())
        return;

    if (offset.x() > 320 || offset.x() < -320)
        return setRange(mView.translated(QPointF(offset.x() * xScale(), offset.y() * yScale())));

    mView.translate(offset.x()*xScale(), offset.y()*yScale());

    foreach (QCPGraph *g, mGraphs) {
        if (offset.x() > 0) {
            double x = mView.right();
            for (int i = 0 ; i <= offset.x() ; i++) {
                g->addData(x, getValue(g->name(), x));
                x += xScale();
            }
            g->data()->removeBefore(mView.left());
        } else if (offset.x() < 0) {
            double x = mView.left();
            for (int i = offset.x() ; i <= 0 ; i++) {
                g->addData(x, getValue(g->name(), x));
                x -= xScale();
            }
            g->data()->removeAfter(mView.right());
        }
    }

    mPlot.xAxis->setRange(mView.left(), mView.right());
    mPlot.yAxis->setRange(mView.top(), mView.bottom());
    redraw();

    if (!mCursorAttached) {
        mCursor->position->setCoords(mView.center());
        mCursor->setVisible(true);
        emit cursorPosChanged(cursorPos());
    }

    emit viewChanged(mView);
}

void CustomPlotItem::moveWindow(int x, int y)
{
    moveWindow(QPoint(x, y));
}

void CustomPlotItem::moveCursor(int amtX, int amtY)
{
    if (mGraphs.isEmpty() || !mCursorAttached)
        return moveWindow(amtX, amtY);

    if (!mCursor->visible()) {
        QPointF cursorPos(mView.center());
        QCPGraph *closest = mCursor->graph();
        double yClo = getValue(mGraphs.first()->name(), cursorPos.x());
        double y;
        foreach (QCPGraph *g, mGraphs) {
            y = getValue(g->name(), cursorPos.x());
            if (std::abs(y - cursorPos.y()) <= std::abs(yClo - cursorPos.y())) {
                closest = g;
                yClo = y;
            }
        }
        mCursor->setGraph(closest);
        mCursor->setGraphKey(cursorPos.x());
        mCursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    }

    if (amtX != 0) {
        mCursor->setGraphKey(cursorPos().x() + amtX * xScale());
        mCursor->updatePosition();
    }

    if (amtY > 0) {
        // search curve just above
        // in case of equality, they are ordered by name (their key in mGraphs)
        // the boolean firstAfter serves this purpose
        QCPGraph *above = mCursor->graph();
        QCPGraph *minig = mCursor->graph();
        double yAbo = std::numeric_limits<double>::max();
        double yMin = std::numeric_limits<double>::max();
        bool firstAfter = false;
        foreach (QCPGraph *g, mGraphs) {
            if (g == mCursor->graph()) {
                firstAfter = true;
                continue;
            }
            const double y = getValue(g->name(), cursorPos().x());
            if (y < yAbo && y > cursorPos().y()) {
                above = g;
                yAbo = y;
            } else if (y == cursorPos().y() && firstAfter) {
                firstAfter = false;
                above = g;
                yAbo = y;
            }
            if (y < yMin) {
                minig = g;
                yMin = y;
            }
        }
        if (mCursor->graph() != above) {
            mCursor->setGraph(above);
        } else {
            mCursor->setGraph(minig);
        }
        mCursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    } else if (amtY < 0) {
        //search curve just below
        QCPGraph *below = mCursor->graph();
        QCPGraph *maxig = mCursor->graph();
        double yBel = -std::numeric_limits<double>::max();
        double yMax = -std::numeric_limits<double>::max();
        bool before = true;
        foreach (QCPGraph *g, mGraphs) {
            if (g == mCursor->graph()) {
                before = false;
                continue;
            }
            const double y = getValue(g->name(), cursorPos().x());
            if ((y > yBel && y < cursorPos().y()) || (y == cursorPos().y() && before)) {
                below = g;
                yBel = y;
            }
            if (y >= yMax) {
                maxig = g;
                yMax = y;
            }
        }
        if (mCursor->graph() != below) {
            mCursor->setGraph(below);
        } else {
            mCursor->setGraph(maxig);
        }
        mCursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    }

    QPointF windowMove;
    // center on cursor if too far away
    if (!mView.contains(cursorPos())) {
        windowMove += cursorPos() - mView.center();
    } else {
        // enforce a margin of 10 pixels
        if (cursorPos().x() > mView.right() - 10 * xScale()) {
            windowMove.rx() += cursorPos().x() + 10 * xScale() - mView.right();
        } else if (cursorPos().x() < mView.left() + 10 * xScale()) {
            windowMove.rx() += cursorPos().x() - 10 * xScale() - mView.left();
        }

        if (cursorPos().y() < mView.top() + 10 * yScale()) {
            windowMove.ry() += cursorPos().y() - 10 * yScale() - mView.top();
        } else if (cursorPos().y() > mView.bottom() - 10 * yScale()) {
            windowMove.ry() += cursorPos().y() + 10 * yScale() - mView.bottom();
        }
    }

    if (!windowMove.isNull()) {
        moveWindow(windowMove.x() / xScale(), windowMove.y() / yScale());
    }

    mCursor->setVisible(true);
    emit cursorPosChanged(cursorPos());
    redraw();
}

void CustomPlotItem::zoomIn(double value)
{
    // change ratio, center stays the same -> modifies X/Y.min/max
    QRectF view;
    view.setSize(mView.size() / value);
    if (value < 1)
        view.moveCenter(mView.center());
    else
        view.moveCenter(cursorPos());

    setRange(view);
}

void CustomPlotItem::updateCustomPlotSize()
{
    // FIXME width() and height() are initially null
    mPlot.setGeometry(0, 0, std::max(width(), 100.), std::max(height(), 100.));

    // the geometry change does not trigger a viewport update immediately
    mPlot.setViewport(mPlot.geometry());

    // dimensions are only updated on replot
    // mPlot.replot();

    replotGraphs();
}

void CustomPlotItem::addGraph(QString id, QColor color)
{
    if (mGraphs.contains(id)) {
        mGraphs[id]->setPen(color);
    } else {
        auto g = mPlot.addGraph();
        g->setPen(color);
        g->setName(id);
        mGraphs[id] = g;
    }
    plotGraph(id);
}

void CustomPlotItem::clearGraph()
{
    for (auto name : mGraphs.keys()) {
        removeGraph(name);
    }
}

void CustomPlotItem::removeGraph(QString name)
{
    if (mGraphs.contains(name)) {
        if (mCursor->graph() == mGraphs[name]) {
            mCursor->setGraph(nullptr);
            mCursor->setVisible(false);
            emit selectedCurveChanged(QString());
        }
        mPlot.removeGraph(mGraphs[name]);
        mGraphs.remove(name);
    }
}

const QRectF &CustomPlotItem::view() const
{
    return mView;
}

QPointF CustomPlotItem::cursorPos() const
{
    return mCursor->position->coords();
}

QString CustomPlotItem::selectedCurve() const
{
    if (mCursor->graph())
        return mCursor->graph()->name();

    return "";
}

void CustomPlotItem::setSelectedCurve(QString curve)
{
    auto it = mGraphs.find(curve);
    if (it != mGraphs.end()) {
        mCursor->setGraph(it.value());
        mCursor->updatePosition();
        emit cursorPosChanged(cursorPos());
        emit selectedCurveChanged(curve);
    } else {
        mCursor->setGraph(nullptr);
        emit selectedCurveChanged(QString());
    }
    redraw();
}

bool CustomPlotItem::isCursorAttached() const
{
    return mCursorAttached;
}

void CustomPlotItem::setCursorAttached(bool attached)
{
    if (attached == mCursorAttached)
        return;
    mCursorAttached = attached;
    if (attached) {
        mCursor->setVisible(false);
    } else {
        mCursor->setGraph(nullptr);
        mCursor->setVisible(true);
        mCursor->position->setCoords(mView.center());
        emit selectedCurveChanged(QString());
        emit cursorPosChanged(mCursor->position->coords());
    }
    emit cursorAttachedChanged(attached);
    redraw();
}

double CustomPlotItem::xScale() const
{
    return 4 * mView.width() / 320;
}

double CustomPlotItem::yScale() const
{
    return 4 * mView.height() / 240;
}

void CustomPlotItem::redraw()
{
    mPlot.replot(QCustomPlot::rpQueuedReplot);
}

double CustomPlotItem::getValue(const QString &f, double x)
{
    giac::gen y = mMathContext->giacEvalString(f)(giac::gen(x), mMathContext->giacContext());

    //if (y.is_real(mMathContext->giacContext()))
    return y.to_double(mMathContext->giacContext()); // returns NaN if y is not real
}
