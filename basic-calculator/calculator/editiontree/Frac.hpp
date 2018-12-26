#ifndef FRAC_HPP
#define FRAC_HPP

#include "EditionNode.hpp"
#include "Flow.hpp"

/** Frac nodes are semantic nodes that represents fractions.
 *
 * A Frac possesses:
 * - a numerator
 * - a denominator
 *
 * Numerator/Denominator are Flow nodes.
 * A Frac node also possesses a boolean cursor_on_top that is set to
 * 'true' if the cursor is in the numerator.
 */
class Frac : public InternalEditionNode
{
public:
    Flow numerator;
    Flow denominator;
    bool cursor_on_top;
public:
    Frac(std::string strnum = "");


    void ascii(int shift, bool contains_cursor) override;
    std::string getText(void) override;

    bool dropCursor(movedir dir) override;

    bool empty(void) override;

    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    EditionNode *getActiveChild(void) override;

    /* About computing dimensions */
    void computeDimensions(QPainter &painter) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;

};

#endif // FRAC_HPP
