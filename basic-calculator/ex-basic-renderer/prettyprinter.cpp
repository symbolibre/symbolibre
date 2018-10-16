#include "prettyprinter.h"

#include <QPainter>
#include <stdlib.h>
#include <algorithm>

PrettyPrinter::PrettyPrinter(QString top_entry, QString bot_entry, QWidget *parent)
    : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    top = top_entry;
    bot = bot_entry;
}

/* About size */
QSize PrettyPrinter::minimumSizeHint() const
{
    return QSize(200, 100);
}

QSize PrettyPrinter::sizeHint() const
{
    return QSize(400, 300);
}

void PrettyPrinter::paintEvent(QPaintEvent * /* */)
{
    /* On veut tracer a+b/ed */
    int x_origin = 50;
    int y_origin = 50;

    QPainter painter(this);
    QFontMetrics metrics = painter.fontMetrics();

    /* Calcul des Bounding rectangles */
    int x_top, y_top, w_top, h_top;
    QRect rect_top = metrics.boundingRect(top);
    rect_top.getRect(&x_top, &y_top, &w_top, &h_top);

    int x_bot, y_bot, w_bot, h_bot;
    QRect rect_bot = metrics.boundingRect(bot);
    rect_bot.getRect(&x_bot, &y_bot, &w_bot, &h_bot);

    int w = std::max(w_top, w_bot);

    /* Barre de fraction */
    QLine line(x_origin, y_origin, x_origin + w, y_origin);
    painter.drawLine(line);

    /* numérateur */
    int dx_n = (w - w_top) / 2;
    int dy_n = -2;
    painter.drawText(x_origin + dx_n, y_origin + dy_n, top);
    painter.drawRect(x_top + x_origin + dx_n, y_top + y_origin + dy_n, w_top, h_top);

    /* denominateur */
    int dx_d = (w - w_bot) / 2;
    int dy_d = h_bot;

    painter.drawText(x_origin + dx_d, y_origin + dy_d, bot);
    painter.drawRect(x_bot + x_origin + dx_d, y_bot + y_origin + dy_d, w_bot, h_bot);
    //printf("%d %d %d %d\n", x,y,w_bot,h_bot);
}
