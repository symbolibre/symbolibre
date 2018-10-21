#include "frac.h"
#include "flow.h"
#include <iostream>
#include <string>
#include <memory>

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
        return numerator->editMoveDown();
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
        return numerator->editDelete();
    else
        return denominator->editDelete();
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
