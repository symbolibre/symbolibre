#include "frac.h"
#include "flow.h"
#include <iostream>
#include <string>
#include <memory>
#include <algorithm>

Frac::Frac(void)
{
    ntype         = FRAC;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;

    cursor_on_top = true;

    /* For now, numerator and denominator are set to empty flows */
    numerator   = std::make_unique<Flow>(FLOW);
    denominator = std::make_unique<Flow>(FLOW);
}

void Frac::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FRACTION\n";
    numerator->ascii(shift + 1, cc && cursor_on_top);
    denominator->ascii(shift + 1, cc && !cursor_on_top);
    return;
}

std::string Frac::get_text(void)
/* FIXME : awfull complexity */
{
    std::string str = " (";
    str.insert(str.size(), numerator->get_text());
    str.insert(str.size(), ")/(");
    str.insert(str.size(), denominator->get_text());
    str.insert(str.size(), ") ");
    return str;
}

void Frac::append(std::string &)
{
    return;
}

bool Frac::drop_cursor(movedir dir)
{
    /* Convention: the cursor is ALWAYS dropped on top of the fraction */
    cursor_on_top = true;
    return numerator->drop_cursor(dir);
}

void Frac::cutAtCursor(std::string &)
{
    return;
}

bool Frac::empty(void)
{
    return numerator->empty() && denominator->empty();
}

bool Frac::reachedRight(void)
{
    if (cursor_on_top)
        return numerator->reachedRight();
    else
        return denominator->reachedRight();
}

bool Frac::reachedLeft(void)
{
    if (cursor_on_top)
        return numerator->reachedLeft();
    else
        return denominator->reachedLeft();
}

bool Frac::editMoveRight(void)
{
    if (cursor_on_top)
        return numerator->editMoveRight();
    else
        return denominator->editMoveRight();
}

bool Frac::editMoveLeft(void)
{
    if (cursor_on_top)
        return numerator->editMoveLeft();
    else
        return denominator->editMoveLeft();
}

bool Frac::editMoveUp(void)
{
    if (cursor_on_top)
        return numerator->editMoveUp();
    /* On bot, if child do not go up, go to numerator-> */
    else if (!denominator->editMoveUp()) {
        cursor_on_top = true;
        numerator->drop_cursor(MLEFT); /* FIXME: smart drop */
        return true;
    } else
        return true; /* child succeed */
}

bool Frac::editMoveDown(void)
{
    if (!cursor_on_top)
        return denominator->editMoveDown();
    /* On top, if child do not go down, go to denominator-> */
    else if (!numerator->editMoveDown()) {
        cursor_on_top = false;
        denominator->drop_cursor(MLEFT); /* FIXME: smart drop */
        return true;
    } else
        return true; /* child succeed */
}

bool Frac::editDelete(void)
{
    if (cursor_on_top)
        numerator->editDelete();
    else
        denominator->editDelete();
    return true;
}

bool Frac::editClear(void)
{
    numerator->editClear();
    denominator->editClear();
}

bool Frac::editChar(char symbol)
{
    if (cursor_on_top)
        return numerator->editChar(symbol);
    else
        return denominator->editChar(symbol);
}

bool Frac::editParen(nodetype paren_type)
{
    if (cursor_on_top)
        return numerator->editParen(paren_type);
    else
        return denominator->editParen(paren_type);
}

bool Frac::editFrac(void)
{
    if (cursor_on_top)
        return numerator->editFrac();
    else
        return denominator->editFrac();
}

void Frac::compute_dimensions(QPainter &painter)
{
    numerator->compute_dimensions(painter);
    denominator->compute_dimensions(painter);

    width  = std::max(numerator->width, denominator->width);
    if (width == 0) {
        QFontMetrics metrics = painter.fontMetrics();
        width = metrics.width(QChar('a'));
    }
    width += FRAC_SPACE;
    height = UP_SPACE + DOWN_SPACE + numerator->height + denominator->height;
    center_height = DOWN_SPACE + denominator->height;

    return;
}

void Frac::draw(int x, int y, QPainter &painter, bool cursor)
{
    QRect brect = QRect(x, y, width, height);

    painter.setPen(Qt::blue);
    //painter.drawRect(brect);

    painter.setPen(Qt::black);

    /* numerator */
    int x_numerator = x + (width - numerator->width) / 2;
    numerator->draw(x_numerator, y, painter, cursor && cursor_on_top);

    /* frac line */
    int y_line = y + height - center_height;
    painter.drawLine(x + FRAC_SPACE / 2, y_line, x - FRAC_SPACE / 2 + width, y_line);

    /* Denominator */
    int x_denominator = x + (width - denominator->width) / 2;
    int y_denominator = y + height - center_height + UP_SPACE;

    denominator->draw(x_denominator, y_denominator, painter,
                      cursor && !cursor_on_top);

    return;
}
