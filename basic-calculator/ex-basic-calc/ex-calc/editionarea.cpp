#include "editionarea.h"

/* ****************************************************************** */
/* **********************       TEXT NODE      ********************** */
/* ****************************************************************** */


EditionArea::EditionArea(void)
{
    ntype         = TEXT;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;
    text          =   "";
}

void EditionArea::ascii(int shift, bool cc)
{
    /* Text nodes have only one child so it's easy */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (cc) {
        std::cout << " └ \"";
        for (int i = 0; i < cursor_pos; i++)
            std::cout << text[i];
        std::cout << "▒" ;
        for (int i = cursor_pos; i < (int)text.size(); i++)
            std::cout << text[i];
        std::cout << "\"\n";
    } else
        std::cout << " └ \"" << text << "\"\n";

    return;
}

void EditionArea::drop_cursor(movedir dir)
{
    /* I could do a switch, but the syntax is ugly on my text editor. */
    if (dir == MRIGHT || dir == MDOWN)
        cursor_pos = text.size();
    else if (dir == MLEFT  || dir == MUP)
        cursor_pos = 0;

    return;
}

bool EditionArea::empty(void)
{
    return text.size() == 0;
}

bool EditionArea::reachedRight(void)
{
    return cursor_pos == (int) text.size();
}
bool EditionArea::reachedLeft(void)
{
    return cursor_pos == 0;
}

bool EditionArea::editMoveRight(void)
{
    if (reachedRight())
        return false;
    else
        cursor_pos ++;
    return true; /* And... this is not ambiguous :-) */
}

bool EditionArea::editMoveLeft(void)
{
    if (reachedLeft())
        return false;
    else
        cursor_pos --;
    return true;
}

bool EditionArea::editMoveUp(void)
{
    cursor_pos = text.size();
    return false;
}

bool EditionArea::editMoveDown(void)
{
    cursor_pos = 0;
    return false;
}

bool EditionArea::editDelete(void)
{
    if (cursor_pos) {
        cursor_pos--;
        text.erase(cursor_pos, 1);
        return true;
    } else
        return false; /* nothing deleted */
}

bool EditionArea::editDigit(int digit)
{
    /* Easy node */
    text.insert(text.begin() + cursor_pos, digit + '0');
    cursor_pos++;
    return true;
}

