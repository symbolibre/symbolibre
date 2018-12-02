#ifndef PAREN_HPP
#define PAREN_HPP

#include "EditionTree.hpp"
#include <string>

/* Parens are semantic nodes, yet a bit special because
 * they are leaf ones. */

class Paren : public EditionTree
{

public: /* methods */
    Paren(nodetype paren_type = LPAREN);

    /* ascii(shift, contains_cursor):
     * Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    void ascii(int shift, bool contains_cursor) override;

    /* getText():
     * Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    std::string getText(void) override;

    /* append(...):
     * Parens does not accept this operation. (Does nothing) */
    void append(std::string &) override;

    /* dropCursor(dir):
     * Parens does not accept this operation. (Does nothing) */
    bool dropCursor(movedir dir) override;

    /* cutAtCursor(cut):
     * Parens does not accept this operation. (Does nothing) */
    void cutAtCursor(std::string &cut) override;

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

    /* editOperator(achar, qchar):
     * Parens does not accept this operation. (Does nothing) */
    bool editOperator(char achar, QString qstring) override;

    bool editParen(nodetype paren_type = LPAREN) override;
    /* Parens does not accept this operation. (Does nothing) */
    bool editFrac(void) override;
    /* Parens does not accept this operation. (Does nothing) */
    /* editRoot():
     * Parens does not accept this operation. (Does nothing) */
    bool editRoot(void) override;

    /* About computing dimensions */
    /* computeDimensions(painter):
     * A paren cannot know its dimensions by itself. The true computation
     * is done by the flow above. */
    void computeDimensions(QPainter &painter) override;

    /* draw(x, y, painter, cursor):
     * Draw the parenthesis with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // PAREN_HPP
