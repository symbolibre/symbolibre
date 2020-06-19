#ifndef FRAC_HPP
#define FRAC_HPP

#include "InternalEditionNode.hpp"
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
    enum {idx_num = 0, idx_den = 1};

    Frac(QString strnum = "");

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
    QPoint getCursorCoordinates(void) override;
};

#endif // FRAC_HPP
