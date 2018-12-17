#ifndef ROOT_HPP
#define ROOT_HPP

#include "InternalEditionNode.hpp"
#include "Flow.hpp"

/* Root are semantical square root nodes (√).
 *
 * They contains a single flow 'expression'.
 * The √ is centered according to 'expression' and has the same height.
 * As the size of the 'root' depends on the size of its 'expression', it holds
 * two int 'rootWidth' and 'rootHeight' that stores the size of the symbol. */

class Root : public InternalEditionNode
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

    /* empty()!
     * Returns 'true' if the current node is empty (see each particular
     * node to check convention. */
    bool empty(void) override;

    /* editClear():
     * Clears the content of the current node. */
    bool editClear(void)  override;

    EditionNode *getActiveChild(void) override;

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
