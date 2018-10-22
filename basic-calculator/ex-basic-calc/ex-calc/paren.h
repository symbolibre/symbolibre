#ifndef PAREN_H
#define PAREN_H

#include "editiontree.h"
#include <string>

/* Parens are semantic nodes, yet a bit special because
 * they are leaf ones. */

#define PAREN_SPACE 4;

class Paren : public EditionTree
{

public: /* methods */
    Paren(nodetype paren_type = LPAREN);
    void ascii(int shift, bool contains_cursor) override;
    std::string get_text(void)                  override;
    void append(std::string &)                   override;

    bool drop_cursor(movedir dir) override;
    void cutAtCursor(std::string &cut) override;
    /* specific to cursor repositioning */

    /* Administrative : two methods that tells if the cursor has space
     * to move right/left. */
    bool empty(void) override;
    bool reachedRight(void) override;
    bool reachedLeft(void) override;

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    bool editMoveRight(void) override;
    bool editMoveLeft(void) override;
    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    /* About some delete actions */
    bool editDelete(void) override;
    bool editClear(void) override;

    bool editChar(char symbol) override;
    bool editParen(nodetype paren_type = LPAREN) override;
    bool editFrac(void) override;

    /* About computing dimensions */
    void compute_dimensions(QPainter &painter) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // PAREN_H
