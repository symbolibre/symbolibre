#ifndef PAREN_HPP
#define PAREN_HPP

#include "InternalEditionNode.hpp"
#include <string>

/** Parens are semantic nodes, yet a bit special because
 * they are leaf ones. */
class Paren : public EditionNode
{
private:
    parentype ptype;

public:
    Paren(parentype paren_type);

    parentype getParenType() const
    {
        return ptype;
    }

    void ascii(int shift, bool contains_cursor) override;
    QString getText(void) const override;
    bool dropCursor(movedir dir) override;
    bool empty(void) const override;

    void computeDimensions(QPainter &painter,
                           qreal lheight, qreal lascent) override;
    void draw(qreal x, qreal y, QPainter &painter, bool cursor) override;
};

#endif // PAREN_HPP
