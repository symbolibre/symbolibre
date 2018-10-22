#include "paren.h"
#include <QString>
#include <algorithm>

Paren::Paren(nodetype paren_type)
{
    ntype         = paren_type;
    width         = 0;
    height        = 0;
    center_height = 0;
    cursor_pos    = 0;
}

void Paren::ascii(int shift, bool)
/* Paren cannot contains the cursor */
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (ntype == LPAREN)
        std::cout << " └ PAREN (\n";
    else
        std::cout << " └ PAREN )\n";
    return;
}

std::string Paren::get_text(void)
{
    std::string str;
    if (ntype == LPAREN)
        str = "(";
    else
        str = ")";
    return str;
}

void Paren::append(std::string &)
{
    return;
}

bool Paren::drop_cursor(movedir)
{
    return false;
}

void Paren::cutAtCursor(std::string &)
{
    return;
}

/* The following methods should not be called */
bool Paren::empty(void)
{
    return false;    /* Contains a parenthesis by defn */
}
bool Paren::reachedRight(void)
{
    return true;
}
bool Paren::reachedLeft(void)
{
    return true;
}
bool Paren::editMoveRight(void)
{
    return false;
}
bool Paren::editMoveLeft(void)
{
    return false;
}
bool Paren::editMoveUp(void)
{
    return false;
}
bool Paren::editMoveDown(void)
{
    return false;
}
bool Paren::editDelete(void)
{
    return false;
}
bool Paren::editClear(void)
{
    return false;
}
bool Paren::editChar(char)
{
    return false;
}
bool Paren::editParen(nodetype)
{
    return false;
}
bool Paren::editFrac(void)
{
    return false;
}

/* Dimensions */
void Paren::compute_dimensions(QPainter &painter)
{
    // FIXME : adjust size
    QFontMetrics metrics = painter.fontMetrics();
    QRect br;
    if (ntype == LPAREN)
        br = metrics.boundingRect(QString("("));
    else
        br = metrics.boundingRect(QString(")"));

    width  = br.width() + PAREN_SPACE;
    height = std::max(MIN_SIZE, br.height());
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
        font_size = MIN_SIZE;

    if (height <= (int)(1.25 * font_size))
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter,
                         ((ntype == LPAREN ? "(" : ")")));
    else if (ntype == LPAREN)
        /* Assume that font is at MIN_SIZE px size */
    {
        brect = QRect(x, y, width, font_size);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignTop, "⎛");

        int y1 = y + height - font_size;
        brect = QRect(x, y1, width, font_size);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, "⎝");

        for (int y0 = y + font_size; y0 < y1 - font_size / 2; y0 += font_size) {
            brect = QRect(x, y0, width, font_size);
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎜");
        }

        /* corrections */
        int y_mid = y + height - 3 * font_size / 2;
        brect = QRect(x, y_mid, width, font_size);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎜");

        if (y1 - 4. / 3. * font_size > y) {
            brect = QRect(x, y1 - (int)(2. / 3. * font_size), width, font_size);
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎜");
        }
    } else if (ntype == RPAREN)
        /* Assume that font is at MIN_SIZE px size */
    {
        brect = QRect(x, y, width, font_size);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignTop, "⎞");

        int y1 = y + height - font_size;
        brect = QRect(x, y1, width, font_size);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, "⎠");

        for (int y0 = y + font_size; y0 < y1 - font_size / 2; y0 += font_size) {
            brect = QRect(x, y0, width, font_size);
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎟");
        }

        /* corrections */
        int y_mid = y + height - 3 * font_size / 2;
        brect = QRect(x, y_mid, width, font_size);
        painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎟");

        if (y1 - 4. / 3. * font_size > y) {
            brect = QRect(x, y1 - (int)(2. / 3. * font_size), width, font_size);
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignVCenter, "⎟");
        }
    }

    return;
}
