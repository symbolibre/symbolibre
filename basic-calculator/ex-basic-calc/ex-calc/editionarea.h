#ifndef EDITIONAREA_H
#define EDITIONAREA_H

#include "editiontree.h"
#include <string>
#include <memory>

class EditionArea : public EditionTree
{
protected:
    std::string text;

public:
    std::string get_text(void);
    void set_to(const char *str);
    void set_to(std::string &str);
    void append(const char *str);
    void append(std::string &str) override;

    void cutAtCursor(std::string &cut);


public:
    EditionArea(void);
    void ascii(int shift, bool contains_cursor) override;
    bool drop_cursor(movedir dir)               override;

    /* Administrative : */
    bool empty(void) override;
    bool reachedRight(void) override;
    bool reachedLeft(void) override;

    /* About moving the cursor */
    bool editMoveRight(void) override;
    bool editMoveLeft(void) override;
    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    /* About deleting */
    bool editDelete(void) override;

    bool editChar(char symbol) override;
    bool editParen(nodetype paren_type = LPAREN) override;
    bool editFrac(void) override;
};

#endif // EDITIONAREA_H
