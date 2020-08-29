#include "Paren.hpp"
#include <QString>
#include <algorithm>

Paren::Paren(parentype paren_type) : EditionNode(),
    ptype(paren_type)
{

}

void Paren::ascii(int shift, bool)
/* Paren cannot contains the cursor */
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (ptype == LPAREN)
        std::cout << " └ PAREN (\n";
    else
        std::cout << " └ PAREN )\n";
}

QString Paren::getText(void) const
{
    QString str;
    if (ptype == LPAREN)
        str = "(";
    else
        str = ")";
    return str;
}

bool Paren::dropCursor(movedir)
{
    return false;
}

/* The following methods should not be called */
bool Paren::empty(void) const
{
    return false;    /* Contains a parenthesis by defn */
}

/* Dimensions */
void Paren::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
{
    // FIXME : adjust size
    QFontMetricsF metrics(painter.font());
    QRectF br;
    if (ptype == LPAREN)
        br = metrics.boundingRect(QString("("));
    else
        br = metrics.boundingRect(QString(")"));

    width  = br.width() + PAREN_SPACE;
    height = std::max(metrics.height(), br.height());
    ascent = metrics.ascent();
}

void Paren::draw(qreal x, qreal y, QPainter &painter, bool)
{
    EditionNode::draw(x, y, painter, false);

    QRectF brect = QRectF(x, y, width, height);
    const qreal font_size = painter.fontInfo().pixelSize();

    if (height <= 2 * font_size)
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter,
                         ((ptype == LPAREN ? "(" : ")")));
    else if (ptype == LPAREN) {
        qreal par_height = std::min(font_size, y + height - font_size / 2);
        brect = QRect(x, y, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignTop, "⎛");

        qreal y1 = y + height - par_height;
        brect = QRect(x, y1, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, "⎝");

        for (qreal y0 = y + font_size; y0 < y1; y0 += font_size) {
            brect = QRect(x, y0, width, std::min(font_size, y1 - y0));
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎜");
        }
    } else if (ptype == RPAREN) {
        qreal par_height = std::min(font_size, y + height - font_size / 2);
        brect = QRect(x, y, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignTop, "⎞");

        qreal y1 = y + height - par_height;
        brect = QRect(x, y1, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, "⎠");

        for (qreal y0 = y + font_size; y0 < y1; y0 += font_size) {
            brect = QRect(x, y0, width, std::min(font_size, y1 - y0));
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎟");
        }
    }
}
