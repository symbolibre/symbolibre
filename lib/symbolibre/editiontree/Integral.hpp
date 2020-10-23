#ifndef INTEGRAL_HPP
#define INTEGRAL_HPP

#include "InternalEditionNode.hpp"
#include "Flow.hpp"
#include "Paren.hpp"

/** Integral nodes represent definite and indefinite integrals.
 *
 * Finite integrals have the integral symbol, two bounds and a body. Indefinite
 * integrals only have a body and a variable node indicating the differential.
 *
 * TODO: Currently the differential is not supported and visually hidden. The
 * variable to integrate for is left to Giac to guess.
 */
class Integral: public InternalEditionNode
{
public:
    qreal int_height;
    qreal int_width;

    // Finite integrals have term, lbound, rbound.
    // Indefinite integrals have term, diff.
    enum {idx_term = 0, idx_lbound = 1, idx_rbound = 2, idx_diff = 1};

public:
    Integral(bool definite);

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
    // Definite or indefinite
    bool m_definite;
    // Parentheses around the term
    Paren m_lpar, m_rpar;
    // Width/height of the integral symbol + lower bound + upper bound group
    qreal m_group_width;
    qreal m_group_height;
};

#endif // INTEGRAL_HPP
