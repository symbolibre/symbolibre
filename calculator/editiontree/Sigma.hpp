#ifndef SIGMA_HPP
#define SIGMA_HPP

#include "EditionNode.hpp"
#include "Flow.hpp"

/** Sigma nodes are semantic nodes that represents finite sums.
 *
 * A Sigma possesses:
 * - a body
 * - a lower bound
 * - an upper bound
 *
 * These are all flow nodes. Anything can be written in these.
 *
 */

class Sigma : public InternalEditionNode
{
public:
    Flow lbound;
    Flow rbound;
    int cursor_location;
    int sigma_height;
    int sigma_width;
public:
    Sigma(void);

    void ascii(int shift, bool contains_cursor) override;
    std::string getText(void) override;

    bool dropCursor(movedir dir) override;

    bool empty(void) override;

    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    EditionNode *getActiveChild(void) override;

    /* About computing dimensions */
    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;

    QPoint getCursorCoordinates(void) override;

};

#endif // SIGMA_HPP
