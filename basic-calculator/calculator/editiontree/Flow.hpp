#ifndef FLOW_H
#define FLOW_H

#include "EditionTree.hpp"

/* This structure is for convenience. */
struct centeredBox {
    int width;
    int height;
    int center_height;
};

/* Flows are a keypart of edidion trees.
 * A Flow contains a list of nodes (typically non-flows). Their properties
 * are described in 'editiontree.h' */

class Flow : public EditionTree
{
private:
    std::list<std::unique_ptr<EditionTree>> flow;
    std::list<std::unique_ptr<EditionTree>>::iterator edited_node;

public:
    Flow(nodetype arg_ntype = FLOW);
    Flow(nodetype arg_ntype, std::string &strinit);
    /* ascii(shift, contains_cursor):
     * Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    void ascii(int shift, bool contains_cursor) override;

    /* getText():
     * Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    std::string getText(void)  override;
    /* append(...):
     * Exclusive to some nodes - don't use this function. */
    void append(std::string &) override;

    /* dropCursor(dir):
     * A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. If the
     * flow is ROOT, catch all 'false' and try to replace the cursor
     * without returning 'false' ever. */
    bool dropCursor(movedir dir) override;

    /* cutAtCursor(&cut):
     * Specific to cursor repositioning. Don't use this function. */
    void cutAtCursor(std::string &cut) override;

    /* Administrative : */
    bool empty(void) override;
    /* Returns 'true' if the current node is empty (see each particular
     * node to check convention. */
    bool reachedRight(void) override;
    /* Tells whether or not the cursor has place to move right. */
    bool reachedLeft(void) override;
    /* Tells whether or not the cursor has place to move left. */

    /* About moving the cursor */
    bool editMoveRight(void) override; /* Move the cursor right. */
    bool editMoveLeft(void) override;  /* Move the cursor left.  */
    bool editMoveUp(void) override;    /* Move the cursor up.    */
    bool editMoveDown(void) override;  /* Move the cursor down.  */

    /* About deleting */
    /* editDelete():
     * Deletes the node or character left to the cursor. Returns 'false'
     * if nothing has been deleted. */
    bool editDelete(void) override;

    /* editClear():
     * Clears the content of the current node. */
    bool editClear(void) override;

    /* The usual edition part */
    /* editChar(symbol):
     * Adds the specified character at the cursor's position. */
    bool editChar(char symbol) override;

    /* editOperator(achar, qchar):
     * Adds the specified operator at the cursor's position, cutting flows if
     * necessary. 'achar' will be the ascii char that will be printed by 'ascii'
     * and 'getText' like methods, and 'qstring' is the pretty string rendered in 2D.
     * Flows ARE the nodes that handle the creation of the operator semantic node. */
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
     * used to call 'draw' method then.
     * Flow nodes also make calls to 'parenArea' to compute the size of
     * parenthesis. */
    void computeDimensions(QPainter &painter) override;

    /* draw(x, y, painter, cursor):
     * Draw the formula with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;

    /* parenArea(...):
     * Returns the box size that the paren at cur_node-- grap. */
    struct centeredBox
    parenArea(std::list<std::unique_ptr<EditionTree>>::iterator &cur_node,
              QPainter &painter);

    /* numberNonEmpty(void):
     * return the number of the nodes which are non-empty within the flow. */
    int numberNonEmpty(void);

};

#endif // FLOW_H
