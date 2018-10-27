#ifndef PAREN_H
#define PAREN_H

#include "EditionTree.hpp"
#include <string>

/* Parens are semantic nodes, yet a bit special because
 * they are leaf ones. */

#define PAREN_SPACE 4;

class Paren : public EditionTree
{

public: /* methods */
    Paren(nodetype paren_type = LPAREN);
    void ascii(int shift, bool contains_cursor) override;
    /* Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    std::string getText(void) override;
    /* Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    void append(std::string &) override;
    /* Parens does not accept this operation. (Does nothing) */

    bool dropCursor(movedir dir) override;
    /* Parens does not accept this operation. (Does nothing) */
    void cutAtCursor(std::string &cut) override;
    /* Parens does not accept this operation. (Does nothing) */

    /* Administrative : two methods that tells if the cursor has space
     * to move right/left. */
    bool empty(void) override;
    bool reachedRight(void) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool reachedLeft(void) override;
    /* Parens does not accept this operation. (Does nothing) */

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    bool editMoveRight(void) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editMoveLeft(void) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editMoveUp(void) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editMoveDown(void) override;
    /* Parens does not accept this operation. (Does nothing) */

    /* About some delete actions */
    bool editDelete(void) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editClear(void) override;
    /* Parens does not accept this operation. (Does nothing) */

    bool editChar(char symbol) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editParen(nodetype paren_type = LPAREN) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editFrac(void) override;
    /* Parens does not accept this operation. (Does nothing) */

    /* About computing dimensions */
    void computeDimensions(QPainter &painter) override;
    /* A paren cannot know its dimensions by itself. The true computation
     * is done by the flow above. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;
    /* Draw the parenthesis with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
};

#endif // PAREN_H
