#ifndef SIGMA_HPP
#define SIGMA_HPP

#include "InternalEditionNode.hpp"
#include "Flow.hpp"
#include "Paren.hpp"

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
    qreal sigma_height;
    qreal sigma_width;

    enum {idx_lbound = 0, idx_rbound = 1, idx_term = 2};

public:
    Sigma(void);

    QString getNodeType() const override;

    void ascii(int shift, bool contains_cursor) override;
    QString getText(void) const override;

    bool dropCursor(movedir dir) override;

    bool empty(void) const override;

    bool editMoveUp(void) override;
    bool editMoveDown(void) override;
    bool editMoveLeft(void) override;
    bool editMoveRight(void) override;

    /* About computing dimensions */
    void computeDimensions(QPainter &painter,
                           qreal lheight, qreal lascent) override;
    void draw(qreal x, qreal y, QPainter &painter, bool cursor) override;

private:
    // Parentheses around the term
    Paren m_lpar, m_rpar;
    // Width of the sum symbol + lower bound + upper bound group
    qreal m_group_width;
};

#endif // SIGMA_HPP
