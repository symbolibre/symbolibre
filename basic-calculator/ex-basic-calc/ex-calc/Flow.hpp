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
    void ascii(int shift, bool contains_cursor) override;
    /* Print the tree structure of the node. 'shift' should be set to 0,
     * contains_cursor to true if you want to track the cursor's position. */
    std::string getText(void)  override;
    /* Returns a std::string that is python-parsable. Beware that this
     * function is sub-optimal. */
    void append(std::string &) override;
    /* Exclusive to some nodes - don't use this function. */
    bool dropCursor(movedir dir) override;
    /* A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. If the
     * flow is ROOT, catch all 'false' and try to replace the cursor
     * without returning 'false' ever. */
    void cutAtCursor(std::string &cut) override;
    /* Specific to cursor repositioning. Don't use this function. */

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
    bool editDelete(void) override;
    /* Deletes the node or character left to the cursor. Returns 'false'
     * if nothing has been deleted. */
    bool editClear(void) override;
    /* Clears the content of the current node. */

    /* The usual edition part */
    bool editChar(char symbol) override;
    /* Adds the specified character to the cursor's position. */
    bool editParen(nodetype paren_type = LPAREN) override;
    /* Adds a paren to the cursor's position. You should specify if this is
     * a left paren (LPAREN) or right one (RPAREN). */
    bool editFrac(void) override;
    /* Adds a fraction to the cursor's position. */

    /* About computing dimensions */
    void computeDimensions(QPainter &painter) override;
    /* Computes the informations necessary to draw the expression:
     * 'width', 'height', 'center_height' for all nodes in the tree.
     * linear complexity. You should give the painter that will be
     * used to call 'draw' method then.
     * Flow nodes also make calls to 'parenArea' to compute the size of
     * parenthesis. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;
    /* Draw the formula with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    struct centeredBox
    parenArea(std::list<std::unique_ptr<EditionTree>>::iterator &cur_node,
              QPainter &painter);
    /* Returns the box size that the paren at cur_node-- grap. */
};

#endif // FLOW_H
