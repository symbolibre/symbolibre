#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "EditionTree.hpp"
#include <QString>

/* Operator are the simplier semantics nodes possible.
 *
 * They have a single operator, that is described using two attributes:
 * - 'qchar' that represents an ascii character corresponding to the operator
 * - 'qstring' that is the 'pretty' string used when rendering the expression in 2D.
 *
 * Operator nodes do not possess any flow, so they are terminal nodes.
 */

class Operator : public EditionTree
{
protected:
    QString qstring;
    char achar;

public:
    Operator(char achar, QString qstring);

    /* ascii(shift, contains_cursor):
     * Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    void ascii(int shift, bool contains_cursor) override;

    /* getText():
     * Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    std::string getText(void) override;

    /* append(str):
     * Exclusive to some nodes - don't use this function. */
    void append(std::string &str) override;

    /* dropCursor(dir):
     * A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. */
    bool dropCursor(movedir dir) override;

    /* cutAtCursor(cut):
     * Specific to cursor repositioning. Don't use this function. */
    void cutAtCursor(std::string &cut) override;

    /* empty():
     * Returns 'true' if the current node is empty.
     * By convention, an operator is never empty. */
    bool empty(void) override;

    /* reachedRight():
     * Tells whether or not the cursor has place to move right.
     * Should not be used. */
    bool reachedRight(void) override;

    /* reachedLeft():
     * Tells whether or not the cursor has place to move left.
     * Should not be used. */
    bool reachedLeft(void) override;

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes.
     * Should not be used. */
    bool editMoveRight(void) override; /* Move the cursor right. */
    bool editMoveLeft(void) override;  /* Move the cursor left.  */
    bool editMoveUp(void) override;    /* Move the cursor up.    */
    bool editMoveDown(void) override;  /* Move the cursor down.  */

    /* About some delete actions */
    /* editDelete():
     * Deletes the node or character left to the cursor.
     * Does nothing and returns false*/
    bool editDelete(void) override;
    /* editClear():
     * Clears the content of the current node. Does nothing on operator nodes. */
    bool editClear(void)  override;

    /* editChar(symbol):
     * Adds the specified character at the cursor's position. */
    bool editChar(char symbol) override;

    /* editOperator(achar, qchar):
     * Adds the specified operator at the cursor's position, cutting flows if
     * necessary. 'achar' will be the ascii char that will be printed by 'ascii'
     * and 'getText' like methods, and 'qstring' is the pretty string rendered in 2D.
     * Should not be used. */
    bool editOperator(char achar, QString qstring) override;

    /* editParen(paren_type):
     * Adds a paren at the cursor's position. You should specify if this is
     * a left paren (LPAREN) or right one (RPAREN).
     * Should not be used. */
    bool editParen(nodetype paren_type = LPAREN) override;

    /* editFrac():
     * Adds a fraction at the cursor's position.
     * Should not be used. */
    bool editFrac(void) override;

    /* editRoot():
     * Adds a root at the cursor's position.
     * Should not be used. */
    bool editRoot(void) override;

    /* About computing dimension to print */
    /* computeDimensions(painter):
     * Computes the informations necessary to draw the expression:
     * 'width', 'height', 'center_height' for all nodes in the tree.
     * linear complexity. You should give the painter that will be
     * used to call 'draw' method then. The size of an operator is constant. */
    void computeDimensions(QPainter &painter) override;

    /* draw(x, y, painter, cursor):
     * Draw the formula with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. Uses 'qstring' to render. Do not take
     * carea about the value of 'cursor'. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // OPERATOR_HPP