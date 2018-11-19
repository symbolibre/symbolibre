#ifndef FRAC_H
#define FRAC_H

/* Frac for "fraction" are semantic nodes.
 * Frac has two specific attributes: numerator and denominator that are flows
 * in practice.
 */

#include "EditionTree.hpp"
#include <memory>

//MIN_SIZE/2

class Flow; /* forward declaration */

/* Frac nodes are semantic nodes that represents fractions.
 *
 * A Frac possesses:
 *
 * - a numerator
 * - a denominator
 *
 * Numerator/Denominator are Flow nodes.
 * A Frac node also possesses a boolean cursor_on_top that is set to
 * 'true' if the cursor is in the numerator. */

class Frac : public EditionTree
{
public:
    std::unique_ptr<Flow> numerator;
    std::unique_ptr<Flow> denominator;
    bool cursor_on_top;
public:
    Frac(void);
    Frac(std::string &strnum);


    void ascii(int shift, bool contains_cursor) override;
    /* Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    std::string getText(void) override;
    /* Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    void append(std::string &str) override;
    /* This function does nothing on Frac nodes. */

    bool dropCursor(movedir dir) override;
    /* A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise.
     * Dropping to the top puts the cursor on the numerator, whereas dropping
     * on the bottom puts the cursor on the denominator (most left part). */

    void cutAtCursor(std::string &cut) override;
    /* This functions does nothing on Frac nodes. */

    bool empty(void) override;
    /* Convention: Fractions are not empty. */
    bool reachedRight(void) override;
    /* Tells whether or not the cursor has place to move right. */
    bool reachedLeft(void) override;
    /* Tells whether or not the cursor has place to move left. */

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
     * Deletes the node or character left to the cursor.
     * Always returns 'true'. A fraction does not propagate the error up if
     * the it cannot erase something. */
    bool editDelete(void) override;

    /* editClear():
     * Clears the numerator and the denominator. */
    bool editClear(void) override;

    /* editChar(symbol);
     * Adds the specified character at the cursor's position. */
    bool editChar(char symbol) override;

    /* editOperator(achar, qchar):
     * Adds the specified operator at the cursor's position, cutting flows if
     * necessary. 'achar' will be the ascii char that will be printed by 'ascii'
     * and 'getText' like methods, and 'qstring' is the pretty string rendered in 2D.
     * Inserts it at the current selected part. */
    bool editOperator(char achar, QString qstring) override;

    /* editParen(paren_type):
     * Adds a paren at the cursor's position. You should specify if this is
     * a left paren (LPAREN) or right one (RPAREN). */
    bool editParen(nodetype paren_type = LPAREN) override;

    /* editFrac():
     * Adds a fraction at the cursor's position. */
    bool editFrac(void) override;

    /* editRoot():
     * Adds a root at the cursor's position. */
    bool editRoot(void) override;

    /* About computing dimensions */
    /* computeDimensions(painter):
     * Computes the informations necessary to draw the expression:
     * 'width', 'height', 'center_height' for all nodes in the tree.
     * linear complexity. You should give the painter that will be
     * used to call 'draw' method then. */
    void computeDimensions(QPainter &painter) override;

    /* draw(x, y, painter, cursor):
     * Draw the fraction with (x,y) being the top left corner of the
     * drawn expression. The fraction bar is always considered at
     * the center. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;

};

#endif // FRAC_H
