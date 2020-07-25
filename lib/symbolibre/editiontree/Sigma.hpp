#ifndef SIGMA_HPP
#define SIGMA_HPP

#include "InternalEditionNode.hpp"
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
    int sigma_height;
    int sigma_width;

    enum {idx_lbound = 0, idx_rbound = 1};

public:
    Sigma(void);

    QString getNodeType() const override;

    void ascii(int shift, bool contains_cursor) override;
    QString getText(void) const override;

    bool dropCursor(movedir dir) override;

    bool empty(void) const override;

    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    /* About computing dimensions */
    void computeDimensions(QPainter &painter,
                           int lheight, int lascent) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // SIGMA_HPP
