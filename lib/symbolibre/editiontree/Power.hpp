#ifndef POWER_HPP
#define POWER_HPP

#include "InternalEditionNode.hpp"
#include "Flow.hpp"

/**
 * Power are semantical power ^{}.
 *
 * They contains a single flow 'expression'.
 * The power is centered such that the bottom line of its expression corresponds
 * to the middle line of its left neighbor in the flow it is located in.
 */
class Power : public InternalEditionNode
{
public:
    Power(void);

    QString getNodeType() const override;

    void ascii(int shift, bool contains_cursor) override;
    QString getText(void) const override;
    bool dropCursor(movedir dir) override;
    bool empty(void) const override;

    void computeDimensions(QPainter &painter,
                           int lheight, int lascent) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // POWER_HPP
