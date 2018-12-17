#ifndef PAREN_HPP
#define PAREN_HPP

#include "EditionNode.hpp"
#include <string>

/* Parens are semantic nodes, yet a bit special because
 * they are leaf ones. */

class Paren : public EditionNode
{
private:
    parentype ptype;

public: /* methods */
    Paren(parentype paren_type);

    parentype getParenType() const
    {
        return ptype;
    }

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

    /* Administrative : two methods that tells if the cursor has space
     * to move right/left. */
    bool empty(void) override;

    /* Parens does not accept this operation. (Does nothing) */
    bool editClear(void) override;

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
