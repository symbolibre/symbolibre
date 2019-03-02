#include "customplotitem.h"
#include "qcustomplot.h"
#include <QDebug>
#include <iostream>
#include <string>

CustomPlotItem::CustomPlotItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_CustomPlot = new QCustomPlot();
    listGraph = {};

    Xcen = 0;
    Ycen = 0;
    Xlen = 10;
    Ylen = 10;

    Xmin = Xcen - Xlen;
    Xmax = Xcen + Xlen;
    Ymin = Ycen - Ylen;
    Ymax = Ycen + Ylen;

    Xsca = 4 * Xlen / 320; // One point every other pixel
    Ysca = 4 * Ylen / 240;

    nbCurves = 0;

    setFlag(QQuickItem::ItemHasContents, true);

    connect(this, &QQuickPaintedItem::widthChanged, this, &CustomPlotItem::updateCustomPlotSize);
    connect(this, &QQuickPaintedItem::heightChanged, this, &CustomPlotItem::updateCustomPlotSize);

    updateCustomPlotSize();
    m_CustomPlot->xAxis->setRange(Xmin, Xmax);
    m_CustomPlot->yAxis->setRange(Ymin, Ymax);
    connect(m_CustomPlot, &QCustomPlot::afterReplot, this, &CustomPlotItem::onCustomReplot);
    m_CustomPlot->replot();
}

CustomPlotItem::~CustomPlotItem()
{
    delete m_CustomPlot;
    m_CustomPlot = nullptr;
}

void CustomPlotItem::paint(QPainter *painter)
{
    // update the Qpainter object
    if (m_CustomPlot) {
        QPixmap    picture(boundingRect().size().toSize());
        QCPPainter qcpPainter(&picture);

        Xmin = Xcen - Xlen;
        Xmax = Xcen + Xlen;
        Ymin = Ycen - Ylen;
        Ymax = Ycen + Ylen;

        m_CustomPlot->xAxis->setRange(Xmin, Xmax);
        m_CustomPlot->yAxis->setRange(Ymin, Ymax);
        m_CustomPlot->replot();

        m_CustomPlot->toPainter(&qcpPainter);

        painter->drawPixmap(QPoint(), picture);
    }
}

void CustomPlotItem::plotGraph(QString nomGraph)
{
    /* take the numero of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    if (m_CustomPlot) {
        auto g = listGraph[nomGraph];

        double x = Xmin;
        g.graph->addData(x, g.getValue(x));
        while (x < Xmax) {
            x += Xsca;
            g.graph->addData(x, g.getValue(x));
        }

        m_CustomPlot->replot();
    }
}

void CustomPlotItem::setRange(double nXmin, double nXmax, double nYmin, double nYmax)
{
    // modify range of the window
    if (m_CustomPlot) {
        Xmin = nXmin;
        Xmax = nXmax;
        Ymin = nYmin;
        Ymax = nYmax;

        Xcen = (Xmin + Xmax) / 2;
        Ycen = (Ymin + Ymax) / 2;
        Xlen = (Xmax - Xmin) / 2;
        Ylen = (Ymax - Ymin) / 2;

        Xsca = 4 * Xlen / 320;
        Ysca = 4 * Ylen / 240;

        for (auto name : listGraph.keys()) {
            plotGraph(name);
        }
    }
}

void CustomPlotItem::moveWindow(int horizontal, int vertical)
{
    //move the window, keep ratio of size, just add an offset
    if (m_CustomPlot) {
        Xcen += Xsca * horizontal;
        Ycen += Ysca * vertical;
        double x;

        foreach (CurveItem g, listGraph) {
            if (horizontal > 0) {
                x = Xmax + Xsca;
                for (int i = 0 ; i < horizontal ; i++) {
                    g.graph->addData(x, g.getValue(x));
                    x += Xsca;
                }
            } else if (horizontal < 0) {
                x = Xmin - Xsca;
                for (int i = horizontal ; i < 0 ; i++) {
                    g.graph->addData(x, g.getValue(x));
                    x -= Xsca;
                }
            }
        }
    }
}

void CustomPlotItem::moveCursor(int amtX, int amtY)
{
    //move cursor according to the amount given
    if (m_CustomPlot) {
        cursorX += amtX;
        cursorY += amtY;
    }
}

void CustomPlotItem::modifyZoom(double value)
{
    // change ratio, center stay the same -> modifies X/Y.min/max
    if (m_CustomPlot) {
        Xlen *= value;
        Ylen *= value;

        Xmin = Xcen - Xlen;
        Xmax = Xcen + Xlen;
        Ymin = Ycen - Ylen;
        Ymax = Ycen + Ylen;

        Xsca *= value;
        Ysca *= value;

        for (auto name : listGraph.keys()) {
            plotGraph(name);
        }
    }
}

void CustomPlotItem::updateCustomPlotSize()
{
    if (m_CustomPlot) {
        m_CustomPlot->setGeometry(0, 0, width(), height());
        m_CustomPlot->setViewport(QRect(0, 0, (int)width(), (int)height()));
    }
}

void CustomPlotItem::recvInput(int input)
{
    switch (input) {
    case KeyCode::SLK_UP:
        moveWindow(0, 1);
        break;
    case KeyCode::SLK_DOWN:
        moveWindow(0, -1);
        break;
    case KeyCode::SLK_LEFT:
        moveWindow(-1, 0);
        break;
    case KeyCode::SLK_RIGHT:
        moveWindow(1, 0);
        break;
    case KeyCode::SLK_PLUS:
        modifyZoom(.5);
        break;
    case KeyCode::SLK_MINUS:
        modifyZoom(2);
        break;
    case KeyCode::SLK_CLEAR:
        clearGraph();
        setRange(-10, 10, -10, 10);
        break;
    default:
        std::cerr << "Unsupported key " << input << std::endl;
        break;
    }
}

void CustomPlotItem::addGraph(QString formula)
{
    if (formula[0] == QChar('-')) {
        removeGraph(formula.remove(0, 1));
        return;
    }

    QStringList decomp = formula.split("(x)=");
    if (listGraph.contains(decomp[0])) {
        listGraph[decomp[0]].updateFormula(decomp[1].toStdString());
    } else {
        listGraph[decomp[0]] = CurveItem(decomp[1].toStdString(), m_CustomPlot->addGraph(), listColor[nbCurves % 4]);
        nbCurves++;
    }
    plotGraph(decomp[0]);
}

void CustomPlotItem::clearGraph()
{
    if (m_CustomPlot) {
        for (auto name : listGraph.keys()) {
            removeGraph(name);
        }
        nbCurves = 0;
    }
}

void CustomPlotItem::removeGraph(QString nomGraph)
{
    if (m_CustomPlot) {
        if (listGraph.contains(nomGraph)) {
            m_CustomPlot->removeGraph(listGraph[nomGraph].graph);
            listGraph.remove(nomGraph);
        }
        nbCurves--;
    }
}

void CustomPlotItem::onCustomReplot()
{
    //qDebug() << Q_FUNC_INFO;
    update();
}
