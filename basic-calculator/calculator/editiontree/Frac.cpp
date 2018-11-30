#include "Frac.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Frac::Frac(std::string strnum) : EditionTree(),
    numerator(FLOW, strnum), denominator()
{
    ntype         = FRAC;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;

    cursor_on_top = true;
}

void Frac::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FRACTION\n";
    numerator.ascii(shift + 1, cc && cursor_on_top);
    denominator.ascii(shift + 1, cc && !cursor_on_top);
    return;
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

void Frac::append(std::string &)
{
    return;
}

bool Frac::dropCursor(movedir dir)
{
    /* Convention: the cursor is ALWAYS dropped on top of the fraction */
    cursor_on_top = true;
    return numerator.dropCursor(dir);
}

void Frac::cutAtCursor(std::string &)
{
    return;
}

bool Frac::empty(void)
{
    return false;
    //return numerator.empty() && denominator.empty();
}

bool Frac::reachedRight(void)
{
    if (cursor_on_top)
        return numerator.reachedRight();
    else
        return denominator.reachedRight();
}

bool Frac::reachedLeft(void)
{
    if (cursor_on_top)
        return numerator.reachedLeft();
    else
        return denominator.reachedLeft();
}

bool Frac::editMoveRight(void)
{
    if (cursor_on_top)
        return numerator.editMoveRight();
    else
        return denominator.editMoveRight();
}

bool Frac::editMoveLeft(void)
{
    if (cursor_on_top)
        return numerator.editMoveLeft();
    else
        return denominator.editMoveLeft();
}

bool Frac::editMoveUp(void)
{
    if (cursor_on_top)
        return numerator.editMoveUp();
    /* On bot, if child do not go up, go to numerator. */
    else if (!denominator.editMoveUp()) {
        cursor_on_top = true;
        numerator.dropCursor(MLEFT); /* FIXME: smart drop */
        return true;
    } else
        return true; /* child succeed */
}

bool Frac::editMoveDown(void)
{
    if (!cursor_on_top)
        return denominator.editMoveDown();
    /* On top, if child do not go down, go to denominator. */
    else if (!numerator.editMoveDown()) {
        cursor_on_top = false;
        denominator.dropCursor(MLEFT); /* FIXME: smart drop */
        return true;
    } else
        return true; /* child succeed */
}

bool Frac::editDelete(void)
{
    if (cursor_on_top)
        numerator.editDelete();
    else
        denominator.editDelete();
    return true;
}

bool Frac::editClear(void)
{
    numerator.editClear();
    denominator.editClear();
    return true;
}

bool Frac::editChar(char symbol)
{
    if (cursor_on_top)
        return numerator.editChar(symbol);
    else
        return denominator.editChar(symbol);
}

bool Frac::editParen(nodetype paren_type)
{
    if (cursor_on_top)
        return numerator.editParen(paren_type);
    else
        return denominator.editParen(paren_type);
}

bool Frac::editFrac(void)
{
    if (cursor_on_top)
        return numerator.editFrac();
    else
        return denominator.editFrac();
}

bool Frac::editRoot(void)
{
    if (cursor_on_top)
        return numerator.editRoot();
    else
        return denominator.editRoot();
}

bool Frac::editOperator(char achar, QString qstring)
{
    if (cursor_on_top)
        return numerator.editOperator(achar, qstring);
    else
        return denominator.editOperator(achar, qstring);
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

    return;
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

    return;
}
