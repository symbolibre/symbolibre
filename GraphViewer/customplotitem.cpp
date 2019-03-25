#include "customplotitem.h"
#include "qcustomplot.h"
#include <QDebug>
#include <iostream>
#include <string>

CustomPlotItem::CustomPlotItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_CustomPlot = new QCustomPlot();

    cursor = new QCPItemTracer(m_CustomPlot);
    cursor->setStyle(QCPItemTracer::tsPlus);
    cursor->setVisible(0);
    modeCursor = 0;
    cursorX = 0;
    cursorY = 0;

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
    /* take the name of graph to plot
     * reset the display
     * compute the new points
     * plot the updated graph
     */
    if (m_CustomPlot) {
        auto g = listGraph[nomGraph];
        g.graph->data()->clear();

        double x = Xmin;
        g.graph->addData(x, g.getValue(x));
        while (x <= Xmax) {
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
        if (listGraph.isEmpty()) {
            std::cout << "No graph to attach the cursor !" << std::endl;
            return;
        }
        cursor->setStyle(QCPItemTracer::tsPlus);
        if (cursor->visible() == 0) {
            cursorX = Xcen;
            cursorY = Ycen;
            QCPGraph *closest = cursor->graph();
            double yClo = listGraph.first().getValue(cursorX);
            double y;
            foreach (CurveItem g, listGraph) {
                y = g.getValue(cursorX);
                if (std::abs(y - cursorY) <= std::abs(yClo - cursorY)) {
                    closest = g.graph;
                    yClo = y;
                }
            }
            cursor->setGraph(closest);
            cursor->setGraphKey(cursorX);
            cursor->updatePosition();
            cursorX = cursor->position->key();
            cursorY = cursor->position->value();
        }

        if (amtX > 0) {
            //bouge le cursor vers la droite
            cursor->setGraphKey(cursorX + amtX * Xsca);
            cursor->updatePosition();
            cursorX = cursor->position->key();
            cursorY = cursor->position->value();
            //si trop a droite shift la
            if (cursorX > Xmax) {
                // centre sur cursor
                double nXmin = cursorX - Xlen;
                double nXmax = cursorX + Xlen;
                double nYmin = cursorY - Ylen;
                double nYmax = cursorY + Ylen;
                setRange(nXmin, nXmax, nYmin, nYmax);
            } else if (cursorX >= Xmax - 10 * Xsca) {
                int mvX = 10 - std::round((Xmax - cursorX) / Xsca);
                moveWindow(mvX, 0);
            }
        } else if (amtX < 0) {
            //bouge le cursor vers la gauche
            cursor->setGraphKey(cursorX + amtX * Xsca);
            cursor->updatePosition();
            cursorX = cursor->position->key();
            cursorY = cursor->position->value();
            //si trop a gauche shift la fenetre
            if (cursorX < Xmin) {
                // centre sur cursor
                double nXmin = cursorX - Xlen;
                double nXmax = cursorX + Xlen;
                double nYmin = cursorY - Ylen;
                double nYmax = cursorY + Ylen;
                setRange(nXmin, nXmax, nYmin, nYmax);
            } else if (cursorX <= Xmin + 10 * Xsca) {
                int mvX = std::round((cursorX - Xmin) / Xsca) - 10;
                moveWindow(mvX, 0);
            }
        }

        if (amtY > 0) {
            //search courbe juste en haut
            QCPGraph *above = cursor->graph();
            QCPGraph *minig = cursor->graph();
            double yAbo = DBL_MAX;
            double yMin = DBL_MAX;
            double y;
            foreach (CurveItem g, listGraph) {
                y = g.getValue(cursorX);
                if (y < yAbo && y > cursorY) {
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
            cursorX = cursor->position->key();
            cursorY = cursor->position->value();

        } else if (amtY < 0) {
            //search courbe juste en bas
            QCPGraph *below = cursor->graph();
            QCPGraph *maxig = cursor->graph();
            double yBel = -DBL_MAX;
            double yMax = -DBL_MAX;
            double y;
            foreach (CurveItem g, listGraph) {
                y = g.getValue(cursorX);
                if (y > yBel && y < cursorY) {
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
            cursorX = cursor->position->key();
            cursorY = cursor->position->value();
        }

        if (cursorY > Ymax) {
            // centre sur cursor
            double nXmin = cursorX - Xlen;
            double nXmax = cursorX + Xlen;
            double nYmin = cursorY - Ylen;
            double nYmax = cursorY + Ylen;
            setRange(nXmin, nXmax, nYmin, nYmax);
        } else if (cursorY >= Ymax - 10 * Ysca) {
            //bouge la window
            int mvY = 10 - std::round((Ymax - cursorY) / Ysca);
            moveWindow(0, mvY);
        } else if (cursorY < Ymin) {
            //centre sur cursor
            double nXmin = cursorX - Xlen;
            double nXmax = cursorX + Xlen;
            double nYmin = cursorY - Ylen;
            double nYmax = cursorY + Ylen;
            setRange(nXmin, nXmax, nYmin, nYmax);
        } else if (cursorY <= Ymin + 10 * Ysca) {
            //bouge window
            int mvY = std::round((cursorY - Ymin) / Ysca) - 10;
            moveWindow(0, mvY);
        }

        cursor->setVisible(1);
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
        setRange(-10, 10, -10, 10);
        break;
    case KeyCode::SLK_ALPHA:
        switchModeCurWin();
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
    }
}

void CustomPlotItem::removeGraph(QString nomGraph)
{
    if (m_CustomPlot) {
        if (listGraph.contains(nomGraph)) {
            //if lié au cursor
            if (cursor->graph() == listGraph[nomGraph].graph) {
                cursor->setGraph(nullptr);
                cursor->setVisible(0);
            }
            m_CustomPlot->removeGraph(listGraph[nomGraph].graph);
            listGraph.remove(nomGraph);
            nbCurves--;
        }
    }
}

void CustomPlotItem::onCustomReplot()
{
    //qDebug() << Q_FUNC_INFO;
    update();
}

double CustomPlotItem::getXmin()
{
    return Xmin;
}

double CustomPlotItem::getXmax()
{
    return Xmax;
}

double CustomPlotItem::getYmax()
{
    return Ymax;
}

double CustomPlotItem::getYmin()
{
    return Ymin;
}

double CustomPlotItem::getCursorX()
{
    return cursorX;
}

double CustomPlotItem::getCursorY()
{
    return cursorY;
}

void CustomPlotItem::setModeWindow()
{
    //now arrows move the window
    modeCursor = 0;
}

void CustomPlotItem::setModeCursor()
{
    //now arrows move the cursor
    modeCursor = 1;
}

void CustomPlotItem::switchModeCurWin()
{
    //switch between moving the window and the cursor
    modeCursor = 1 - modeCursor;
}