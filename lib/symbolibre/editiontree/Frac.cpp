#include "Frac.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Frac::Frac(QString strnum) : InternalEditionNode(2)
{
    children[0] = Flow(strnum);
}

QString Frac::getNodeType() const
{
    return "frac";
}

void Frac::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FRACTION\n";
    children[idx_num].ascii(shift + 1, cc && active_child_idx == idx_num);
    children[idx_den].ascii(shift + 1, cc && active_child_idx == idx_den);
}

QString Frac::getText(void) const
/* FIXME : awfull complexity */
{
    QString str = "(";
    str.insert(str.size(), children[idx_num].getText());
    str.insert(str.size(), ")/(");
    str.insert(str.size(), children[idx_den].getText());
    str.insert(str.size(), ")");
    return str;
}

bool Frac::dropCursor(movedir dir)
{
    /* Convention: the cursor is ALWAYS dropped on top of the fraction */
    active_child_idx = idx_num;
    return children[idx_num].dropCursor(dir);
}

bool Frac::empty(void) const
{
    return false;
    //return children[idx_num].empty() && children[idx_den].empty();
}

bool Frac::editMoveUp(void)
{
    if (active_child_idx == idx_num)
        return false;

    active_child_idx = idx_num;
    children[idx_num].dropCursor(MUP);
    return true;
}

bool Frac::editMoveDown(void)
{
    if (active_child_idx == idx_den)
        return false;

    active_child_idx = idx_den;
    children[idx_den].dropCursor(MDOWN);
    return true;
}

void Frac::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    children[idx_num].computeDimensions(painter, 0, 0);
    children[idx_den].computeDimensions(painter, 0, 0);

    QFontMetrics metrics = painter.fontMetrics();

    width  = std::max(children[idx_num].width, children[idx_den].width);

    width += FRAC_SPACE;
    height = FRAC_UP_SPACE + FRAC_DOWN_SPACE +
             children[idx_num].height + children[idx_den].height;
    // FIXME is there a better way to align the fraction?
    ascent = FRAC_UP_SPACE + children[idx_num].height + metrics.ascent() / 3;
}

void Frac::draw(int x, int y, QPainter &painter, bool cursor)
{
    // Numerator
    int x_numerator = x + (width - children[idx_num].width) / 2;
    children[idx_num].draw(x_numerator, y, painter, cursor && active_child_idx == idx_num);

    // frac line
    int y_line = y + children[idx_num].height + FRAC_UP_SPACE;
    painter.drawLine(x, y_line, x + width - 1, y_line);

    // Denominator
    int x_denominator = x + (width - children[idx_den].width) / 2;
    int y_denominator = y_line + FRAC_DOWN_SPACE;

    children[idx_den].draw(x_denominator, y_denominator, painter,
                           cursor && active_child_idx == idx_den);
}
