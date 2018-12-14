#ifndef ROOT_HPP
#define ROOT_HPP

#include "EditionTree.hpp"
#include "Flow.hpp"

/* Root are semantical square root nodes (√).
 *
 * They contains a single flow 'expression'.
 * The √ is centered according to 'expression' and has the same height.
 * As the size of the 'root' depends on the size of its 'expression', it holds
 * two int 'rootWidth' and 'rootHeight' that stores the size of the symbol. */

class Root : public EditionTree
{
private:
    Flow expression;
    int rootWidth;

public:
    Root(void);  /* I'm Root */

    /* ascii(shift, contains_cursor):
     * Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    void ascii(int shift, bool contains_cursor) override;

    /* getText():
     * Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    std::string getText(void) override;

    /* append(str): this function does nothing on Root nodes. */
    void append(std::string &str) override;

    /* dropCursor(dir):
     * A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. */
    bool dropCursor(movedir dir) override;

    /* cutAtCursor(cut):
     * Specific to cursor repositioning. Don't use this function. */
    void cutAtCursor(std::string &cut) override;

    /* empty()!
     * Returns 'true' if the current node is empty (see each particular
     * node to check convention. */
    bool empty(void) override;

    /* reachedRight():
     * Tells whether or not the cursor has place to move right. */
    bool reachedRight(void) override;

    /* reachedLeft():
     * Tells whether or not the cursor has place to move left. */
    bool reachedLeft(void) override;

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    bool editMoveRight(void) override; /* Move the cursor right. */
    bool editMoveLeft(void) override;  /* Move the cursor left.  */
    bool editMoveUp(void) override;    /* Move the cursor up.    */
    bool editMoveDown(void) override;  /* Move the cursor down.  */

    /* About some delete actions */
    /* editDelete():
     * Deletes the node or character left to the cursor. Returns 'false'
     * if nothing has been deleted. */
    bool editDelete(void) override;
    /* editClear():
     * Clears the content of the current node. */
    bool editClear(void)  override;

    /* editChar(symbol):
     * Adds the specified character at the cursor's position. */
    bool editChar(char symbol) override;

    /* editOperator(achar, qchar):
     * Adds the specified operator at the cursor's position, cutting flows if
     * necessary. 'achar' will be the ascii char that will be printed by 'ascii'
     * and 'getText' like methods, and 'qstring' is the pretty string rendered in 2D. */
    bool editOperator(char achar, QString qstring) override;

    /* editParen(paren_type):
     * Adds a paren at the cursor's position. You should specify if this is
     * a left paren (LPAREN) or right one (RPAREN). */
    bool editParen(parentype paren_type = LPAREN) override;

    /* editFrac():
     * Adds a fraction at the cursor's position. */
    bool editFrac(void) override;

    /* editRoot(void):
     * Adds a square at the cursor's position. */
    bool editRoot(void) override;

    EditionTree *getActiveNode(void) override;

    /* About computing dimension to print */

    /* computeDimensions(painter):
     * Computes the informations necessary to draw the expression:
     * 'width', 'height', 'center_height' for all nodes in the tree.
     * linear complexity. You should give the painter that will be
     * used to call 'draw' method then. */
    void computeDimensions(QPainter &painter) override;

    /* draw(x, y, painter, cursor):
     * Draw the formula with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;

};

#endif // ROOT_HPP
