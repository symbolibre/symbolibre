#include "customplotitem.h"
#include "qcustomplot.h"
#include <QDebug>
#include <iostream>
#include <string>

CustomPlotItem::CustomPlotItem(QQuickItem *parent) : QQuickPaintedItem(parent),
    m_CustomPlot(), m_view(-10, -10, 20, 20), m_cursorPos(0, 0)
{
    cursor = new QCPItemTracer(&m_CustomPlot);
    cursor->setStyle(QCPItemTracer::tsPlus);
    cursor->setVisible(0);
    modeCursor = 1;

    listGraph = {};

    Xsca = 4 * m_view.width() / 320; // One point every other pixel
    Ysca = 4 * m_view.height() / 240;

    nbCurves = 0;

    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize);

    updateCustomPlotSize();
    m_CustomPlot.xAxis->setRange(m_view.left(), m_view.right());
    m_CustomPlot.yAxis->setRange(m_view.top(), m_view.bottom());
    connect(&m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot);
    m_CustomPlot.replot();
}

CustomPlotItem::~CustomPlotItem()
{

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
    g.graph->addData(x, g.getValue(x));
    while (x <= m_view.right()) {
        x += Xsca;
        g.graph->addData(x, g.getValue(x));
    }

    m_CustomPlot.replot();
}

void CustomPlotItem::setRange(const QRectF &range)
{
    m_view = range;

    Xsca = 4 * m_view.width() / 320;
    Ysca = 4 * m_view.height() / 240;

    for (auto name : listGraph.keys()) {
        plotGraph(name);
    }
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
        return setRange(m_view.translated(QPointF(offset.x() * Xsca, offset.y() * Ysca)));

    foreach (CurveItem g, listGraph) {
        if (offset.x() > 0) {
            double x = m_view.right();
            for (int i = 0 ; i <= offset.x() ; i++) {
                g.graph->addData(x, g.getValue(x));
                x += Xsca;
            }
        } else if (offset.x() < 0) {
            double x = m_view.left();
            for (int i = offset.x() ; i <= 0 ; i++) {
                g.graph->addData(x, g.getValue(x));
                x -= Xsca;
            }
        }
    }

    m_view.translate(offset.x()*Xsca, offset.y()*Ysca);

    if (modeCursor == 0) {
        cursor->position->setCoords(m_view.center());
        cursor->setVisible(1);
        m_cursorPos = m_view.center();
        emit cursorPosChanged(m_cursorPos);
    }

    emit viewChanged(m_view);
}

void CustomPlotItem::moveWindow(int x, int y)
{
    moveWindow(QPoint(x, y));
}

void CustomPlotItem::moveCursor(int amtX, int amtY)
{
    //move cursor according to the amount given
    if (listGraph.isEmpty()) {
        std::cout << "No graph to attach the cursor !" << std::endl;
        return;
    }
    cursor->setStyle(QCPItemTracer::tsPlus);
    if (!cursor->visible()) {
        m_cursorPos = m_view.center();
        QCPGraph *closest = cursor->graph();
        double yClo = listGraph.first().getValue(m_cursorPos.x());
        double y;
        foreach (CurveItem g, listGraph) {
            y = g.getValue(m_cursorPos.x());
            if (std::abs(y - m_cursorPos.y()) <= std::abs(yClo - m_cursorPos.y())) {
                closest = g.graph;
                yClo = y;
            }
        }
        cursor->setGraph(closest);
        cursor->setGraphKey(m_cursorPos.x());
        cursor->updatePosition();
        m_cursorPos.rx() = cursor->position->key();
        m_cursorPos.ry() = cursor->position->value();
        emit selectedCurveChanged(selectedCurve());
    }

    if (amtX != 0) {
        cursor->setGraphKey(m_cursorPos.x() + amtX * Xsca);
        cursor->updatePosition();
        m_cursorPos.setX(cursor->position->key());
        m_cursorPos.setY(cursor->position->value());
    }

    if (amtY > 0) {
        // search curve just above
        QCPGraph *above = cursor->graph();
        QCPGraph *minig = cursor->graph();
        double yAbo = DBL_MAX;
        double yMin = DBL_MAX;
        double y;
        foreach (CurveItem g, listGraph) {
            y = g.getValue(m_cursorPos.x());
            if (y < yAbo && y > m_cursorPos.y()) {
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
        m_cursorPos.rx() = cursor->position->key();
        m_cursorPos.ry() = cursor->position->value();
        emit selectedCurveChanged(selectedCurve());
    } else if (amtY < 0) {
        //search curve just below
        QCPGraph *below = cursor->graph();
        QCPGraph *maxig = cursor->graph();
        double yBel = -DBL_MAX;
        double yMax = -DBL_MAX;
        double y;
        foreach (CurveItem g, listGraph) {
            y = g.getValue(m_cursorPos.x());
            if (y > yBel && y < m_cursorPos.y()) {
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
        m_cursorPos.rx() = cursor->position->key();
        m_cursorPos.ry() = cursor->position->value();
        emit selectedCurveChanged(selectedCurve());
    }

    QPointF windowMove;
    // center on cursor if too far away
    if (!m_view.contains(m_cursorPos)) {
        windowMove += m_cursorPos - m_view.center();
    } else {
        // enforce a margin of 10 pixels
        if (m_cursorPos.x() > m_view.right() - 10 * Xsca) {
            windowMove.rx() += m_cursorPos.x() + 10 * Xsca - m_view.right();
        } else if (m_cursorPos.x() < m_view.left() + 10 * Xsca) {
            windowMove.rx() += m_cursorPos.x() - 10 * Xsca - m_view.left();
        }

        if (m_cursorPos.y() < m_view.top() + 10 * Ysca) {
            windowMove.ry() += m_cursorPos.y() - 10 * Ysca - m_view.top();
        } else if (m_cursorPos.y() > m_view.bottom() - 10 * Ysca) {
            windowMove.ry() += m_cursorPos.y() + 10 * Ysca - m_view.bottom();
        }
    }

    if (!windowMove.isNull()) {
        moveWindow(windowMove.x() / Xsca, windowMove.y() / Ysca);
    }

    cursor->setVisible(1);
    emit cursorPosChanged(m_cursorPos);
}

void CustomPlotItem::modifyZoom(double value)
{
    // change ratio, center stay the same -> modifies X/Y.min/max
    const auto center = m_view.center();
    m_view.setSize(m_view.size() * value);
    if (value > 1)
        m_view.moveCenter(center);
    else
        m_view.moveCenter(m_cursorPos);

    Xsca *= value;
    Ysca *= value;

    for (auto name : listGraph.keys()) {
        plotGraph(name);
    }
}

void CustomPlotItem::updateCustomPlotSize()
{
    m_CustomPlot.setGeometry(0, 0, width(), height());
    m_CustomPlot.setViewport(QRect(0, 0, (int)width(), (int)height()));
}

void CustomPlotItem::recvInput(int input)
{
    switch (input) {
    case KeyCode::SLK_UP:
        if (modeCursor == 0) {
            moveWindow(0, 1);
        } else {
            moveCursor(0, 1);
        }
        break;
    case KeyCode::SLK_DOWN:
        if (modeCursor == 0) {
            moveWindow(0, -1);
        } else {
            moveCursor(0, -1);
        }
        break;
    case KeyCode::SLK_LEFT:
        if (modeCursor == 0) {
            moveWindow(-1, 0);
        } else {
            moveCursor(-1, 0);
        }
        break;
    case KeyCode::SLK_RIGHT:
        if (modeCursor == 0) {
            moveWindow(1, 0);
        } else {
            moveCursor(1, 0);
        }
        break;
    case KeyCode::SLK_PLUS:
        modifyZoom(.5);
        break;
    case KeyCode::SLK_MINUS:
        modifyZoom(2);
        break;
    case KeyCode::SLK_CLEAR:
        clearGraph();
        setRange(QRectF(-10, -10, 20, 20));
        break;
    case KeyCode::SLK_ALPHA:
        switchModeCurWin();
        break;
    default:
        std::cerr << "Unsupported key " << input << std::endl;
        break;
    }
}

void CustomPlotItem::addGraph(QString formula, QColor color)
{
    if (!formula.count()) {
        std::cerr << "empty formula given to addGraph\n";
        return;
    }

    if (formula[0] == QChar('-')) {
        removeGraph(formula.remove(0, 1));
        return;
    }

    QStringList decomp = formula.split("(x)=");
    if (decomp.size() != 2) {
        std::cerr << "FIXME non-trivial function'" << formula.toStdString() << "' in addGraph\n";
        return;
    }
    if (listGraph.contains(decomp[0])) {
        listGraph[decomp[0]].updateFormula(decomp[1].toStdString());
    } else {
        listGraph[decomp[0]] = CurveItem(decomp[1].toStdString(), m_CustomPlot.addGraph(), color);
        nbCurves++;
    }
    plotGraph(decomp[0]);
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
        //if lié au cursor
        if (cursor->graph() == listGraph[nomGraph].graph) {
            cursor->setGraph(nullptr);
            cursor->setVisible(0);
            emit selectedCurveChanged(QString());
        }
        m_CustomPlot.removeGraph(listGraph[nomGraph].graph);
        listGraph.remove(nomGraph);
        nbCurves--;
    }
}

void CustomPlotItem::onCustomReplot()
{
    update();
}

const QRectF &CustomPlotItem::view() const
{
    return m_view;
}

QPointF CustomPlotItem::cursorPos() const
{
    return m_cursorPos;
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
        m_cursorPos.rx() = cursor->position->key();
        m_cursorPos.ry() = cursor->position->value();
        emit cursorPosChanged(m_cursorPos);
        emit selectedCurveChanged(curve);
    } else {
        cursor->setGraph(nullptr);
        emit selectedCurveChanged(QString());
    }
}

void CustomPlotItem::setModeWindow()
{
    //now arrows move the window
    modeCursor = 0;

    // We free the cursor from the graph
    cursor->setGraph(nullptr);
    cursor->position->setCoords(m_view.center());
    cursor->setVisible(1);
    emit selectedCurveChanged(QString());
    emit cursorPosChanged(m_cursorPos);
}

void CustomPlotItem::setModeCursor()
{
    //now arrows move the cursor
    modeCursor = 1;
    cursor->setVisible(0);
}

void CustomPlotItem::switchModeCurWin()
{
    //switch between moving the window and the cursor
    modeCursor = 1 - modeCursor;
}
