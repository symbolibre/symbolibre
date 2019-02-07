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

std::string Paren::getText(void)
{
    std::string str;
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
bool Paren::empty(void)
{
    return false;    /* Contains a parenthesis by defn */
}

/* Dimensions */
void Paren::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    // FIXME : adjust size
    QFontMetrics metrics = painter.fontMetrics();
    QRect br;
    if (ptype == LPAREN)
        br = metrics.boundingRect(QString("("));
    else
        br = metrics.boundingRect(QString(")"));

    width  = br.width() + PAREN_SPACE;
    height = std::max(FONT_SIZE, br.height());
    center_height = height / 2; // FIXME ?
}

void Paren::draw(int x, int y, QPainter &painter, bool)
{
    QRect brect = QRect(x, y, width, height);

    /*painter.setPen(Qt::green);
    painter.drawRect(brect);
    painter.setPen(Qt::black);*/

    //std::cerr << "Paren height:" << height << "\n";

    QFont font = painter.font();
    int font_size = font.pixelSize();
    if (font_size < 0)
        font_size = FONT_SIZE;

    if (height <= (int)(2 * font_size))
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter,
                         ((ptype == LPAREN ? "(" : ")")));
    else if (ptype == LPAREN)
        /* Assume that font is at FONT_SIZE px size */
    {
        int par_height = std::min(font_size, y + height - font_size / 2);
        brect = QRect(x, y, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignTop, "⎛");

        int y1 = y + height - par_height;
        brect = QRect(x, y1, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, "⎝");

        for (int y0 = y + font_size; y0 < y1; y0 += font_size) {
            brect = QRect(x, y0, width, std::min(font_size, y1 - y0));
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎜");
        }
    } else if (ptype == RPAREN)
        /* Assume that font is at FONT_SIZE px size */
    {
        int par_height = std::min(font_size, y + height - font_size / 2);
        brect = QRect(x, y, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignTop, "⎞");

        int y1 = y + height - par_height;
        brect = QRect(x, y1, width, par_height);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, "⎠");

        for (int y0 = y + font_size; y0 < y1; y0 += font_size) {
            brect = QRect(x, y0, width, std::min(font_size, y1 - y0));
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎟");
        }
    }
}
