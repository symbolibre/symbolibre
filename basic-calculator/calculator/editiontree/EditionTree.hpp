#ifndef EDITIONTREE_HPP
#define EDITIONTREE_HPP

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QRect>

#include "graphicalParameters.hpp"

/* A brief introduction to EDITION TREES
 *
 *
 * Edition Trees are a structure thought to handle the interactive edition of a
 * mathematical formula, while having enough structure to allow an on-the-run
 * beautifying of the previous in reasonable time.
 *
 * Edition Trees are classic trees with three kinds of nodes:
 * - edition areas : leaves which contain a single string.
 * - semantic nodes: these nodes are the axis to beautify the expression; these
 *                   are fractions, parenthesis, square roots, integrals...
 * - flows         : these are undefinitely long chains of edition areas and
 *                   semantic nodes that correspond to a line block.
 *
 * An edition tree has also a *cursor*. The cursor points exclusively to an
 * edition area that is the one being currently edited. Thus, any input will
 * modify the structure at this specific edition area.
 *
 * Here are some properties that an edition tree satifies:
 * (1) flows begin and end with edition areas,
 * (2) flows contain nodes and edition areas, nodes contain flows, edition
 *     areas are terminal,
 * (3) two semantic nodes in a flow have to be separated by an edition area,
 * (4) there cannot be two consecutive edition areas in a flow,
 * (5) the cursor always point to an edition area.
 *
 *
 *
 *
 * EditionTree is a virtual class, and you should not use it directly.
 * To init a classical edition tree, you may want a flow node, so it
 * is adviced to use code such that:
 *
 * EditionTree my_expression = Flow(ROOT);
 *
 */

enum movedir { MNONE, MRESET, MLEFT, MRIGHT, MUP, MDOWN };
enum nodetype { NONE, ROOT, FLOW, TEXT, OPERATOR, LPAREN, RPAREN, FRAC, SQUAREROOT };

class EditionTree /* Edition tree represents a set of class, and should
                   * not be used alone. */
{
public:
    /* Edition trees have the following attributes:
     *
     * - `ntype` that represents the type of the node
     * - `width, height, center_height` that are attributes to draw the
          expression, that are quite explainatory.
     * - `cursor_pos` that is the id of the sub-node containing the position
     *    of the cursor. */
    nodetype ntype;

    int width;
    int height;
    int center_height;

    int cursor_pos;

public:
    virtual ~EditionTree() = default;

    /* ascii(shift, contains_cursor):
     * Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    virtual void ascii(int shift, bool contains_cursor) = 0;

    /* getText():
     * Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    virtual std::string getText(void) = 0;

    /* append(str): Exclusive to some nodes - don't use this function. */
    virtual void append(std::string &str) = 0;

    /* dropCursor(dir):
     * A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. */
    virtual bool dropCursor(movedir dir) = 0;

    /* cutAtCursor(cut):
     * Specific to cursor repositioning. Don't use this function. */
    virtual void cutAtCursor(std::string &cut) = 0;

    /* empty():
     * Returns 'true' if the current node is empty.
     * By convention, a root node is empty iff its content is empty. */
    virtual bool empty(void) = 0;

    /* reachedRight():
     * Tells whether or not the cursor has room to move right. */
    virtual bool reachedRight(void) = 0;

    /* reachedLeft():
     * Tells whether or not the cursor has room to move left. */
    virtual bool reachedLeft(void) = 0;

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    virtual bool editMoveRight(void) = 0; /* Move the cursor right. */
    virtual bool editMoveLeft(void) = 0;  /* Move the cursor left.  */
    virtual bool editMoveUp(void) = 0;    /* Move the cursor up.    */
    virtual bool editMoveDown(void) = 0;  /* Move the cursor down.  */

    /* About some delete actions */
    /* editDelete():
     * Deletes the node or character left to the cursor. Returns 'false'
     * if nothing has been deleted. */
    virtual bool editDelete(void) = 0;
    /* editClear():
     * Clears the content of the current node. */
    virtual bool editClear(void)  = 0;

    /* editChar(symbol):
     * Adds the specified character at the cursor's position. */
    virtual bool editChar(char symbol) = 0;

    /* editOperator(achar, qchar):
     * Adds the specified operator at the cursor's position, cutting flows if
     * necessary. 'achar' will be the ascii char that will be printed by 'ascii'
     * and 'getText' like methods, and 'qstring' is the pretty string rendered in 2D.
     * Flows ARE the nodes that handle the creation of the operator semantic node. */
    virtual bool editOperator(char achar, QString qstring) = 0;

    /* editParen(paren_type):
     * Adds a paren at the cursor's position. You should specify if this is
     * a left paren (LPAREN) or right one (RPAREN). */
    virtual bool editParen(nodetype paren_type = LPAREN) = 0;

    /* editFrac():
     * Adds a fraction at the cursor's position. */
    virtual bool editFrac(void) = 0;

    /* editRoot():
     * Adds a root at the cursor's position. */
    virtual bool editRoot(void) = 0;

    /** Returns the leaf where the cursor is. */
    virtual EditionTree *getActiveNode(void)
    {
        return this;
    }

    /* About computing dimension to print */
    /* computeDimensions(painter):
     * Computes the informations necessary to draw the expression:
     * 'width', 'height', 'center_height' for all nodes in the tree.
     * linear complexity. You should give the painter that will be
     * used to call 'draw' method then.
    * Flow nodes also make calls to 'parenArea' to compute the size of
     * parentheses. */
    virtual void computeDimensions(QPainter &painter) = 0;

    /* draw(x, y, painter, cursor):
     * Draw the formula with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    virtual void draw(int x, int y, QPainter &painter, bool cursor) = 0;
};

#endif // EDITIONTREE_HPP
