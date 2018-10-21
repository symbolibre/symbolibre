#include "editionarea.h"

/* ****************************************************************** */
/* **********************       TEXT NODE      ********************** */
/* ****************************************************************** */


EditionArea::EditionArea()
{
    ntype         = TEXT;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;
    text          =   "";
}

std::string EditionArea::get_text(void)
{
    return text;
}

void EditionArea::set_to(const char *str)
{
    text = str;
}
void EditionArea::set_to(std::string &str)
{
    text = str;
}
void EditionArea::append(const char *str)
{
    text.insert(text.size(), str);
}
void EditionArea::append(std::string &str)
{
    text.insert(text.size(), str);
}

void EditionArea::cutAtCursor(std::string &cut)
{
    cut = text.substr(cursor_pos, text.size() - cursor_pos);
    text.erase(cursor_pos, text.size() - cursor_pos);
    return;
}

void EditionArea::ascii(int shift, bool cc)
{
    /* Text nodes have only one child so it's easy */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (cc) {
        std::cout << " └*\"";
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

bool EditionArea::drop_cursor(movedir dir)
{
    /* I could do a switch, but the syntax is ugly on my text editor. */
    if (dir == MRIGHT || dir == MDOWN)
        cursor_pos = text.size();
    else if (dir == MLEFT  || dir == MUP)
        cursor_pos = 0;

    return true;
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

bool EditionArea::editChar(char symbol)
{
    /* Easy node */
    text.insert(text.begin() + cursor_pos, symbol);
    cursor_pos++;
    return true;
}

/* An edition area cannot manage the insertion of a semantic alone, because
 * a split has to be done */
bool EditionArea::editParen(nodetype)
{
    return false;
}
bool EditionArea::editFrac(void)
{
    return false;
}

