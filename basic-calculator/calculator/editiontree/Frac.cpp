#include "Frac.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Frac::Frac(std::string strnum) : InternalEditionNode(),
    numerator(strnum), denominator(), cursor_on_top(true)
{

}

void Frac::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FRACTION\n";
    numerator.ascii(shift + 1, cc && cursor_on_top);
    denominator.ascii(shift + 1, cc && !cursor_on_top);
}

std::string Frac::getText(void)
/* FIXME : awfull complexity */
{
    std::string str = "(";
    str.insert(str.size(), numerator.getText());
    str.insert(str.size(), ")/(");
    str.insert(str.size(), denominator.getText());
    str.insert(str.size(), ")");
    return str;
}

bool Frac::dropCursor(movedir dir)
{
    /* Convention: the cursor is ALWAYS dropped on top of the fraction */
    cursor_on_top = true;
    return numerator.dropCursor(dir);
}

bool Frac::empty(void)
{
    return false;
    //return numerator.empty() && denominator.empty();
}

bool Frac::editMoveUp(void)
{
    if (cursor_on_top)
        return false;

    cursor_on_top = true;
    numerator.dropCursor(MLEFT); /* FIXME: smart drop */
    return true;
}

bool Frac::editMoveDown(void)
{
    if (!cursor_on_top)
        return false;

    cursor_on_top = false;
    denominator.dropCursor(MLEFT); /* FIXME: smart drop */
    return true;
}

bool Frac::editClear(void)
{
    numerator.editClear();
    denominator.editClear();
    return true;
}

EditionNode *Frac::getActiveChild(void)
{
    if (cursor_on_top)
        return &numerator;
    return &denominator;
}

void Frac::computeDimensions(QPainter &painter)
{
    numerator.computeDimensions(painter);
    denominator.computeDimensions(painter);

    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString("0"));
    int min_height = br.height();

    width  = std::max(numerator.width, denominator.width);
    if (width == 0)
        width = metrics.width(QChar('0'));

    width += FRAC_SPACE;
    height = FRAC_UP_SPACE + FRAC_DOWN_SPACE +
             std::max(min_height, numerator.height) +
             std::max(min_height, denominator.height);
    center_height = FRAC_DOWN_SPACE +
                    std::max(min_height, denominator.height);
}

void Frac::draw(int x, int y, QPainter &painter, bool cursor)
{
    /*QRect brect = QRect(x, y, width, height);

    painter.setPen(Qt::red);
    painter.drawRect(brect);
    painter.setPen(Qt::black);*/

    /* numerator */
    int x_numerator = x + (width - numerator.width) / 2;
    numerator.draw(x_numerator, y, painter, cursor && cursor_on_top);

    /* frac line */
    int y_line = y + height - center_height;
    painter.drawLine(x + FRAC_SPACE / 2, y_line, x - FRAC_SPACE / 2 + width - 1, y_line);

    /* Denominator */
    int x_denominator = x + (width - denominator.width) / 2;
    int y_denominator = y + height - center_height + FRAC_UP_SPACE;

    denominator.draw(x_denominator, y_denominator, painter,
                     cursor && !cursor_on_top);
}
