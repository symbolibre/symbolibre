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
private:
    Flow expression;

public:
    Power(void);  /* I'm Root */

    void ascii(int shift, bool contains_cursor) override;
    std::string getText(void) override;
    bool dropCursor(movedir dir) override;
    bool empty(void) override;

    EditionNode *getActiveChild(void) override;

    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
    QPoint getCursorCoordinates(void) override;

};

#endif // POWER_HPP
