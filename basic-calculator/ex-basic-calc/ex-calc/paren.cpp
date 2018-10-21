#include "paren.h"

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
