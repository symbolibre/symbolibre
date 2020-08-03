#include "customplotitem.h"

#include <limits>
#include <qcustomplot.h>
#include "symbolibre/cas/MathContext.hpp"

CustomPlotItem::CustomPlotItem(QQuickItem *parent) : QQuickPaintedItem(parent),
    mMathContext(nullptr), m_CustomPlot(), m_view(-10, -10, 20, 20),
    mCursorAttached(false), cursor(new QCPItemTracer(&m_CustomPlot)), listGraph()
{
    cursor->setStyle(QCPItemTracer::tsPlus);
    cursor->setVisible(false);

    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize);

    updateCustomPlotSize();
    m_CustomPlot.xAxis->setRange(m_view.left(), m_view.right());
    m_CustomPlot.yAxis->setRange(m_view.top(), m_view.bottom());
    connect(&m_CustomPlot, SIGNAL(afterReplot()), this, SLOT(update()));
    redraw();
}

void CustomPlotItem::paint(QPainter *painter)
{
    QPixmap    picture(boundingRect().size().toSize());
    QCPPainter qcpPainter(&picture);
    m_CustomPlot.toPainter(&qcpPainter);
    painter->drawPixmap(QPoint(), picture);
}

void CustomPlotItem::plotGraph(QString f)
{
    /* take the name of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    auto g = listGraph[f];
    g->data()->clear();

    double x = m_view.left();
    g->addData(x, getValue(f, x));
    while (x <= m_view.right()) {
        x += xScale();
        g->addData(x, getValue(f, x));
    }

    redraw();
}

void CustomPlotItem::replotGraphs()
{
    for (const QString &f : listGraph.keys())
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
    if (range == m_view)
        return;

    m_view = range;
    m_CustomPlot.xAxis->setRange(m_view.left(), m_view.right());
    m_CustomPlot.yAxis->setRange(m_view.top(), m_view.bottom());

    replotGraphs();
    emit viewChanged(m_view);
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
        return setRange(m_view.translated(QPointF(offset.x() * xScale(), offset.y() * yScale())));

    foreach (QCPGraph *g, listGraph) {
        if (offset.x() > 0) {
            double x = m_view.right();
            for (int i = 0 ; i <= offset.x() ; i++) {
                g->addData(x, getValue(g->name(), x));
                x += xScale();
            }
        } else if (offset.x() < 0) {
            double x = m_view.left();
            for (int i = offset.x() ; i <= 0 ; i++) {
                g->addData(x, getValue(g->name(), x));
                x -= xScale();
            }
        }
    }

    m_view.translate(offset.x()*xScale(), offset.y()*yScale());
    m_CustomPlot.xAxis->setRange(m_view.left(), m_view.right());
    m_CustomPlot.yAxis->setRange(m_view.top(), m_view.bottom());
    redraw();

    if (!mCursorAttached) {
        cursor->position->setCoords(m_view.center());
        cursor->setVisible(true);
        emit cursorPosChanged(cursorPos());
    }

    emit viewChanged(m_view);
}

void CustomPlotItem::moveWindow(int x, int y)
{
    moveWindow(QPoint(x, y));
}

void CustomPlotItem::moveCursor(int amtX, int amtY)
{
    if (listGraph.isEmpty() || !mCursorAttached)
        return moveWindow(amtX, amtY);

    if (!cursor->visible()) {
        QPointF cursorPos(m_view.center());
        QCPGraph *closest = cursor->graph();
        double yClo = getValue(listGraph.first()->name(), cursorPos.x());
        double y;
        foreach (QCPGraph *g, listGraph) {
            y = getValue(g->name(), cursorPos.x());
            if (std::abs(y - cursorPos.y()) <= std::abs(yClo - cursorPos.y())) {
                closest = g;
                yClo = y;
            }
        }
        cursor->setGraph(closest);
        cursor->setGraphKey(cursorPos.x());
        cursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    }

    if (amtX != 0) {
        cursor->setGraphKey(cursorPos().x() + amtX * xScale());
        cursor->updatePosition();
    }

    if (amtY > 0) {
        // search curve just above
        // in case of equality, they are ordered by name (their key in listGraph)
        // the boolean firstAfter serves this purpose
        QCPGraph *above = cursor->graph();
        QCPGraph *minig = cursor->graph();
        double yAbo = std::numeric_limits<double>::max();
        double yMin = std::numeric_limits<double>::max();
        bool firstAfter = false;
        foreach (QCPGraph *g, listGraph) {
            if (g == cursor->graph()) {
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
        if (cursor->graph() != above) {
            cursor->setGraph(above);
        } else {
            cursor->setGraph(minig);
        }
        cursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    } else if (amtY < 0) {
        //search curve just below
        QCPGraph *below = cursor->graph();
        QCPGraph *maxig = cursor->graph();
        double yBel = -std::numeric_limits<double>::max();
        double yMax = -std::numeric_limits<double>::max();
        bool before = true;
        foreach (QCPGraph *g, listGraph) {
            if (g == cursor->graph()) {
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
        if (cursor->graph() != below) {
            cursor->setGraph(below);
        } else {
            cursor->setGraph(maxig);
        }
        cursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    }

    QPointF windowMove;
    // center on cursor if too far away
    if (!m_view.contains(cursorPos())) {
        windowMove += cursorPos() - m_view.center();
    } else {
        // enforce a margin of 10 pixels
        if (cursorPos().x() > m_view.right() - 10 * xScale()) {
            windowMove.rx() += cursorPos().x() + 10 * xScale() - m_view.right();
        } else if (cursorPos().x() < m_view.left() + 10 * xScale()) {
            windowMove.rx() += cursorPos().x() - 10 * xScale() - m_view.left();
        }

        if (cursorPos().y() < m_view.top() + 10 * yScale()) {
            windowMove.ry() += cursorPos().y() - 10 * yScale() - m_view.top();
        } else if (cursorPos().y() > m_view.bottom() - 10 * yScale()) {
            windowMove.ry() += cursorPos().y() + 10 * yScale() - m_view.bottom();
        }
    }

    if (!windowMove.isNull()) {
        moveWindow(windowMove.x() / xScale(), windowMove.y() / yScale());
    }

    cursor->setVisible(true);
    emit cursorPosChanged(cursorPos());
    redraw();
}

void CustomPlotItem::zoomIn(double value)
{
    // change ratio, center stays the same -> modifies X/Y.min/max
    QRectF view;
    view.setSize(m_view.size() / value);
    if (value < 1)
        view.moveCenter(m_view.center());
    else
        view.moveCenter(cursorPos());

    setRange(view);
}

void CustomPlotItem::updateCustomPlotSize()
{
    // FIXME width() and height() are initially null
    m_CustomPlot.setGeometry(0, 0, std::max(width(), 100.), std::max(height(), 100.));

    // the geometry change does not trigger a viewport update immediately
    m_CustomPlot.setViewport(m_CustomPlot.geometry());

    // dimensions are only updated on replot
    // m_CustomPlot.replot();

    replotGraphs();
}

void CustomPlotItem::addGraph(QString id, QColor color)
{
    if (listGraph.contains(id)) {
        listGraph[id]->setPen(color);
    } else {
        auto g = m_CustomPlot.addGraph();
        g->setPen(color);
        g->setName(id);
        listGraph[id] = g;
    }
    plotGraph(id);
}

void CustomPlotItem::clearGraph()
{
    for (auto name : listGraph.keys()) {
        removeGraph(name);
    }
}

void CustomPlotItem::removeGraph(QString name)
{
    if (listGraph.contains(name)) {
        if (cursor->graph() == listGraph[name]) {
            cursor->setGraph(nullptr);
            cursor->setVisible(false);
            emit selectedCurveChanged(QString());
        }
        m_CustomPlot.removeGraph(listGraph[name]);
        listGraph.remove(name);
    }
}

const QRectF &CustomPlotItem::view() const
{
    return m_view;
}

QPointF CustomPlotItem::cursorPos() const
{
    return cursor->position->coords();
}

QString CustomPlotItem::selectedCurve() const
{
    if (cursor->graph())
        return cursor->graph()->name();

    return "";
}

void CustomPlotItem::setSelectedCurve(QString curve)
{
    auto it = listGraph.find(curve);
    if (it != listGraph.end()) {
        cursor->setGraph(it.value());
        cursor->updatePosition();
        emit cursorPosChanged(cursorPos());
        emit selectedCurveChanged(curve);
    } else {
        cursor->setGraph(nullptr);
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
        cursor->setVisible(false);
    } else {
        cursor->setGraph(nullptr);
        cursor->setVisible(true);
        cursor->position->setCoords(m_view.center());
        emit selectedCurveChanged(QString());
        emit cursorPosChanged(cursor->position->coords());
    }
    emit cursorAttachedChanged(attached);
    redraw();
}

double CustomPlotItem::xScale() const
{
    return 4 * m_view.width() / 320;
}

double CustomPlotItem::yScale() const
{
    return 4 * m_view.height() / 240;
}

void CustomPlotItem::redraw()
{
    m_CustomPlot.replot(QCustomPlot::rpQueuedReplot);
}

double CustomPlotItem::getValue(const QString &f, double x)
{
    giac::gen y = mMathContext->giacEvalString(f)(giac::gen(x), mMathContext->giacContext());

    //if (y.is_real(mMathContext->giacContext()))
    return y.to_double(mMathContext->giacContext()); // returns NaN if y is not real
}
