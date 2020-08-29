#ifndef OPERATOR_HPP
#define OPERATOR_HPP

#include "InternalEditionNode.hpp"
#include <QString>

/**
 * Operator are the simplest semantic nodes possible.
 *
 * They have a single operator, that is described using two attributes:
 * - 'achar' that represents an ascii character corresponding to the operator
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

    void ascii(int shift, bool contains_cursor) override;
    QString getText(void) const override;
    bool dropCursor(movedir dir) override;
    bool empty(void) const override;

    void computeDimensions(QPainter &painter,
                           qreal lheight, qreal lascent) override;
    void draw(qreal x, qreal y, QPainter &painter, bool cursor) override;
};

#endif // OPERATOR_HPP
