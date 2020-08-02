#include "customplotitem.h"

#include <limits>
#include <qcustomplot.h>

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
    m_CustomPlot.replot();
}

void CustomPlotItem::paint(QPainter *painter)
{
    // update the Qpainter object
    QPixmap    picture(boundingRect().size().toSize());
    QCPPainter qcpPainter(&picture);

    m_CustomPlot.xAxis->setRange(m_view.left(), m_view.right());
    m_CustomPlot.yAxis->setRange(m_view.top(), m_view.bottom());
    m_CustomPlot.replot();

    m_CustomPlot.toPainter(&qcpPainter);

    painter->drawPixmap(QPoint(), picture);
}

void CustomPlotItem::plotGraph(QString nomGraph)
{
    /* take the name of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    auto g = listGraph[nomGraph];
    g.graph->data()->clear();

    double x = m_view.left();
    g.graph->addData(x, g.getValue(x, mMathContext));
    while (x <= m_view.right()) {
        x += xScale();
        g.graph->addData(x, g.getValue(x, mMathContext));
    }

    m_CustomPlot.replot();
}

void CustomPlotItem::replot()
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
    replot();
    emit mathContextChanged(ctx);
}

void CustomPlotItem::setRange(const QRectF &range)
{
    if (range == m_view)
        return;

    m_view = range;

    replot();
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

    foreach (CurveItem g, listGraph) {
        if (offset.x() > 0) {
            double x = m_view.right();
            for (int i = 0 ; i <= offset.x() ; i++) {
                g.graph->addData(x, g.getValue(x, mMathContext));
                x += xScale();
            }
        } else if (offset.x() < 0) {
            double x = m_view.left();
            for (int i = offset.x() ; i <= 0 ; i++) {
                g.graph->addData(x, g.getValue(x, mMathContext));
                x -= xScale();
            }
        }
    }

    m_view.translate(offset.x()*xScale(), offset.y()*yScale());

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
        double yClo = listGraph.first().getValue(cursorPos.x(), mMathContext);
        double y;
        foreach (CurveItem g, listGraph) {
            y = g.getValue(cursorPos.x(), mMathContext);
            if (std::abs(y - cursorPos.y()) <= std::abs(yClo - cursorPos.y())) {
                closest = g.graph;
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
        QCPGraph *above = cursor->graph();
        QCPGraph *minig = cursor->graph();
        double yAbo = std::numeric_limits<double>::max();
        double yMin = std::numeric_limits<double>::max();
        double y;
        foreach (CurveItem g, listGraph) {
            y = g.getValue(cursorPos().x(), mMathContext);
            if (y < yAbo && y > cursorPos().y()) {
                above = g.graph;
                yAbo = y;
            }
            if (y < yMin) {
                minig = g.graph;
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
        double yBel = std::numeric_limits<double>::min();
        double yMax = std::numeric_limits<double>::min();
        double y;
        foreach (CurveItem g, listGraph) {
            y = g.getValue(cursorPos().x(), mMathContext);
            if (y > yBel && y < cursorPos().y()) {
                below = g.graph;
                yBel = y;
            }
            if (y > yMax) {
                maxig = g.graph;
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
}

void CustomPlotItem::modifyZoom(double value)
{
    // change ratio, center stay the same -> modifies X/Y.min/max
    const auto center = m_view.center();
    m_view.setSize(m_view.size() * value);
    if (value > 1)
        m_view.moveCenter(center);
    else
        m_view.moveCenter(cursorPos());

    for (auto name : listGraph.keys()) {
        plotGraph(name);
    }
}

void CustomPlotItem::updateCustomPlotSize()
{
    m_CustomPlot.setGeometry(0, 0, width(), height());
    m_CustomPlot.setViewport(QRect(0, 0, (int)width(), (int)height()));
}

void CustomPlotItem::addGraph(QString id, QColor color)
{
    if (listGraph.contains(id)) {
        listGraph[id].setColor(color);
    } else {
        listGraph[id] = CurveItem(id, m_CustomPlot.addGraph(), color);
    }
    plotGraph(id);
}

void CustomPlotItem::clearGraph()
{
    for (auto name : listGraph.keys()) {
        removeGraph(name);
    }
}

void CustomPlotItem::removeGraph(QString nomGraph)
{
    if (listGraph.contains(nomGraph)) {
        if (cursor->graph() == listGraph[nomGraph].graph) {
            cursor->setGraph(nullptr);
            cursor->setVisible(false);
            emit selectedCurveChanged(QString());
        }
        m_CustomPlot.removeGraph(listGraph[nomGraph].graph);
        listGraph.remove(nomGraph);
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
    for (auto it = listGraph.begin(); it != listGraph.end(); ++it)
        if (it.value().graph == cursor->graph())
            return it.key();
    return "";
}

void CustomPlotItem::setSelectedCurve(QString curve)
{
    auto it = listGraph.find(curve);
    if (it != listGraph.end()) {
        cursor->setGraph(it->graph);
        cursor->updatePosition();
        emit cursorPosChanged(cursorPos());
        emit selectedCurveChanged(curve);
    } else {
        cursor->setGraph(nullptr);
        emit selectedCurveChanged(QString());
    }
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
}

double CustomPlotItem::xScale() const
{
    return 4 * m_view.width() / 320;
}

double CustomPlotItem::yScale() const
{
    return 4 * m_view.height() / 240;
}
