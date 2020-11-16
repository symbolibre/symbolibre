#include "SLQuickGraph.hpp"

#include <limits>
#include <qcustomplot.h>
#include "symbolibre/cas/MathContext.hpp"

SLQuickGraph::SLQuickGraph(QQuickItem *parent) : QQuickPaintedItem(parent),
    m_mathContext(nullptr), m_plot(), m_graphs(), m_cursor(new QCPItemTracer(&m_plot)),
    m_view(-10, -10, 20, 20), m_cursorAttached(false)
{
    m_cursor->setStyle(QCPItemTracer::tsPlus);
    m_cursor->setVisible(false);

    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, &QQuickPaintedItem::widthChanged, this, &SLQuickGraph::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &SLQuickGraph::updateCustomPlotSize);

    updateCustomPlotSize();
    m_plot.xAxis->setRange(m_view.left(), m_view.right());
    m_plot.yAxis->setRange(m_view.top(), m_view.bottom());
    connect(&m_plot, SIGNAL(afterReplot()), this, SLOT(update()));
    redraw();
}

void SLQuickGraph::paint(QPainter *painter)
{
    QPixmap    picture(boundingRect().size().toSize());
    QCPPainter qcpPainter(&picture);
    m_plot.toPainter(&qcpPainter);
    painter->drawPixmap(QPoint(), picture);
}

void SLQuickGraph::plotGraph(QString f)
{
    /* take the name of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    auto g = m_graphs[f];
    g->data()->clear();

    bool err;

    double x = m_view.left();
    double y = getValue(f, x, &err);
    if (!err)
        g->addData(x, y);

    while (x <= m_view.right()) {
        x += xScale();
        y = getValue(f, x, &err);
        if (!err)
            g->addData(x, y);
    }

    redraw();
}

void SLQuickGraph::replotGraphs()
{
    for (const QString &f : m_graphs.keys())
        plotGraph(f);
}

MathContext * SLQuickGraph::mathContext() const
{
    return m_mathContext;
}

void SLQuickGraph::setMathContext(MathContext *ctx)
{
    if (ctx == m_mathContext)
        return;

    m_mathContext = ctx;
    replotGraphs();
    emit mathContextChanged(ctx);
}

void SLQuickGraph::setRange(const QRectF &range)
{
    if (range == m_view)
        return;

    m_view = range;
    m_plot.xAxis->setRange(m_view.left(), m_view.right());
    m_plot.yAxis->setRange(m_view.top(), m_view.bottom());

    replotGraphs();
    emit viewChanged(m_view);
}

void SLQuickGraph::setRange(qreal xmin, qreal xmax, qreal ymin, qreal ymax)
{
    setRange(QRectF(xmin, ymin, xmax - xmin, ymax - ymin));
}

void SLQuickGraph::moveWindow(QPoint offset)
{
    //move the window, keep ratio of size, just add an offset
    if (offset.isNull())
        return;

    if (offset.x() > 320 || offset.x() < -320)
        return setRange(m_view.translated(QPointF(offset.x() * xScale(), offset.y() * yScale())));

    m_view.translate(offset.x()*xScale(), offset.y()*yScale());

    bool err;

    foreach (QCPGraph *g, m_graphs) {
        if (offset.x() > 0) {
            double x = m_view.right();
            for (int i = 0 ; i <= offset.x() ; i++) {
                double y = getValue(g->name(), x, &err);
                if (!err)
                    g->addData(x, y);
                x += xScale();
            }
            g->data()->removeBefore(m_view.left());
        } else if (offset.x() < 0) {
            double x = m_view.left();
            for (int i = offset.x() ; i <= 0 ; i++) {
                double y = getValue(g->name(), x, &err);
                if (!err)
                    g->addData(x, y);
                x -= xScale();
            }
            g->data()->removeAfter(m_view.right());
        }
    }

    m_plot.xAxis->setRange(m_view.left(), m_view.right());
    m_plot.yAxis->setRange(m_view.top(), m_view.bottom());
    redraw();

    if (!m_cursorAttached) {
        m_cursor->position->setCoords(m_view.center());
        m_cursor->setVisible(true);
        emit cursorPosChanged(cursorPos());
    }

    emit viewChanged(m_view);
}

void SLQuickGraph::moveWindow(int x, int y)
{
    moveWindow(QPoint(x, y));
}

void SLQuickGraph::moveCursor(int amtX, int amtY)
{
    if (m_graphs.isEmpty() || !m_cursorAttached)
        return moveWindow(amtX, amtY);

    if (!m_cursor->visible()) {
        QPointF cursorPos(m_view.center());
        QCPGraph *closest = m_cursor->graph();
        double yClo = getValue(m_graphs.first()->name(), cursorPos.x());
        double y;
        foreach (QCPGraph *g, m_graphs) {
            y = getValue(g->name(), cursorPos.x());
            if (std::abs(y - cursorPos.y()) <= std::abs(yClo - cursorPos.y())) {
                closest = g;
                yClo = y;
            }
        }
        m_cursor->setGraph(closest);
        m_cursor->setGraphKey(cursorPos.x());
        m_cursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    }

    if (amtX != 0) {
        m_cursor->setGraphKey(cursorPos().x() + amtX * xScale());
        m_cursor->updatePosition();
    }

    if (amtY > 0) {
        // search curve just above
        // in case of equality, they are ordered by name (their key in m_graphs)
        // the boolean firstAfter serves this purpose
        QCPGraph *above = m_cursor->graph();
        QCPGraph *minig = m_cursor->graph();
        double yAbo = std::numeric_limits<double>::max();
        double yMin = std::numeric_limits<double>::max();
        bool firstAfter = false;
        foreach (QCPGraph *g, m_graphs) {
            if (g == m_cursor->graph()) {
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
        if (m_cursor->graph() != above) {
            m_cursor->setGraph(above);
        } else {
            m_cursor->setGraph(minig);
        }
        m_cursor->updatePosition();
        emit selectedCurveChanged(selectedCurve());
    } else if (amtY < 0) {
        //search curve just below
        QCPGraph *below = m_cursor->graph();
        QCPGraph *maxig = m_cursor->graph();
        double yBel = -std::numeric_limits<double>::max();
        double yMax = -std::numeric_limits<double>::max();
        bool before = true;
        foreach (QCPGraph *g, m_graphs) {
            if (g == m_cursor->graph()) {
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
        if (m_cursor->graph() != below) {
            m_cursor->setGraph(below);
        } else {
            m_cursor->setGraph(maxig);
        }
        m_cursor->updatePosition();
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

    m_cursor->setVisible(true);
    emit cursorPosChanged(cursorPos());
    redraw();
}

void SLQuickGraph::zoomIn(double value)
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

void SLQuickGraph::addError(QString msg)
{
    // Avoid repeating messages; important because functions are evaluated at
    // typically tens of points and the errors often repeat
    if (!m_errors.contains(msg)) {
        m_errors.append(msg);
        emit errorsChanged(m_errors);
    }
}

void SLQuickGraph::clearErrors()
{
    bool changed = m_errors.size() > 0;
    m_errors.clear();
    if (changed)
        emit errorsChanged(m_errors);
}

void SLQuickGraph::updateCustomPlotSize()
{
    // FIXME width() and height() are initially null
    m_plot.setGeometry(0, 0, std::max(width(), 100.), std::max(height(), 100.));

    // the geometry change does not trigger a viewport update immediately
    m_plot.setViewport(m_plot.geometry());

    // dimensions are only updated on replot
    // m_plot.replot();

    replotGraphs();
}

void SLQuickGraph::addGraph(QString id, QColor color)
{
    if (m_graphs.contains(id)) {
        m_graphs[id]->setPen(color);
    } else {
        auto g = m_plot.addGraph();
        g->setPen(color);
        g->setName(id);
        m_graphs[id] = g;
    }
    plotGraph(id);
}

void SLQuickGraph::clearGraph()
{
    for (auto name : m_graphs.keys()) {
        removeGraph(name);
    }
}

void SLQuickGraph::removeGraph(QString name)
{
    if (m_graphs.contains(name)) {
        if (m_cursor->graph() == m_graphs[name]) {
            m_cursor->setGraph(nullptr);
            m_cursor->setVisible(false);
            emit selectedCurveChanged(QString());
        }
        m_plot.removeGraph(m_graphs[name]);
        m_graphs.remove(name);
    }
}

const QRectF &SLQuickGraph::view() const
{
    return m_view;
}

QPointF SLQuickGraph::cursorPos() const
{
    return m_cursor->position->coords();
}

QString SLQuickGraph::selectedCurve() const
{
    if (m_cursor->graph())
        return m_cursor->graph()->name();

    return "";
}

void SLQuickGraph::setSelectedCurve(QString curve)
{
    auto it = m_graphs.find(curve);
    if (it != m_graphs.end()) {
        m_cursor->setGraph(it.value());
        m_cursor->updatePosition();
        emit cursorPosChanged(cursorPos());
        emit selectedCurveChanged(curve);
    } else {
        m_cursor->setGraph(nullptr);
        emit selectedCurveChanged(QString());
    }
    redraw();
}

bool SLQuickGraph::isCursorAttached() const
{
    return m_cursorAttached;
}

const QStringList &SLQuickGraph::errors() const
{
    return m_errors;
}

void SLQuickGraph::setCursorAttached(bool attached)
{
    if (attached == m_cursorAttached)
        return;
    m_cursorAttached = attached;
    if (attached) {
        m_cursor->setVisible(false);
    } else {
        m_cursor->setGraph(nullptr);
        m_cursor->setVisible(true);
        m_cursor->position->setCoords(m_view.center());
        emit selectedCurveChanged(QString());
        emit cursorPosChanged(m_cursor->position->coords());
    }
    emit cursorAttachedChanged(attached);
    redraw();
}

double SLQuickGraph::xScale() const
{
    return 4 * m_view.width() / 320;
}

double SLQuickGraph::yScale() const
{
    return 4 * m_view.height() / 240;
}

void SLQuickGraph::redraw()
{
    m_plot.replot(QCustomPlot::rpQueuedReplot);
}

double SLQuickGraph::getValue(const QString &f, double x, bool *err)
{
    giac::gen y;

    try {
        y = m_mathContext->giacEvalString(f)(giac::gen(x), m_mathContext->giacContext());
        if (err)
            *err = false;
    }
    catch (std::exception &e) {
        addError(f + ":" + QString::fromStdString(e.what()));
        y = giac::gen(0);
        if (err)
            *err = true;
    }

    //if (y.is_real(m_mathContext->giacContext()))
    return y.to_double(m_mathContext->giacContext()); // returns NaN if y is not real
}
