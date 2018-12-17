#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "EditionNode.hpp"
#include <QString>

/* Operator are the simplier semantics nodes possible.
 *
 * They have a single operator, that is described using two attributes:
 * - 'qchar' that represents an ascii character corresponding to the operator
 * - 'qstring' that is the 'pretty' string used when rendering the expression in 2D.
 *
 * Operator nodes do not possess any flow, so they are terminal nodes.
 */

class Operator : public EditionNode
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

    /* dropCursor(dir):
     * A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. */
    bool dropCursor(movedir dir) override;

    /* empty():
     * Returns 'true' if the current node is empty.
     * By convention, an operator is never empty. */
    bool empty(void) override;

    /* editClear():
     * Clears the content of the current node. Does nothing on operator nodes. */
    bool editClear(void)  override;

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
