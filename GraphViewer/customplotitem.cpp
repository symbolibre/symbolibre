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

void CustomPlotItem::plotGraph(int numGraph)
{
    /* take the numero of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    QColor listColor[4] = {Qt::red, Qt::blue, Qt::green, Qt::black};
    if (m_CustomPlot) {
        if (numGraph >= nbCurves) {
            m_CustomPlot->addGraph();
            nbCurves++;
        } else {
            m_CustomPlot->graph(numGraph)->data()->clear();
        }
        m_CustomPlot->graph(numGraph)->setPen(QPen(listColor[numGraph % 4]));

        double x = Xmin;
        m_CustomPlot->graph(numGraph)->addData(x, listGraph[numGraph].getValue(x));
        while (x < Xmax) {
            x += Xsca;
            m_CustomPlot->graph(numGraph)->addData(x, listGraph[numGraph].getValue(x));
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

        for (int j = 0 ; j < nbCurves ; j++) {
            plotGraph(j);
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

        for (int j = 0 ; j < nbCurves ; j++) {
            if (horizontal > 0) {
                x = Xmax + Xsca;
                for (int i = 0 ; i < horizontal ; i++) {
                    m_CustomPlot->graph(j)->addData(x, listGraph[j].getValue(x));
                    x += Xsca;
                }
            } else if (horizontal < 0) {
                x = Xmin - Xsca;
                for (int i = horizontal ; i < 0 ; i++) {
                    m_CustomPlot->graph(j)->addData(x, listGraph[j].getValue(x));
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

        for (int j = 0 ; j < nbCurves ; j++) {
            plotGraph(j);
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
    default:
        std::cerr << "Unsupported key " << input << std::endl;
        break;
    }
}

void CustomPlotItem::addGraph(QString formula)
{
    listGraph.append(CurveItem(formula.toStdString()));
    plotGraph(nbCurves);
}

void CustomPlotItem::onCustomReplot()
{
    //qDebug() << Q_FUNC_INFO;
    update();
}
