#ifndef PAREN_HPP
#define PAREN_HPP

#include "EditionNode.hpp"
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
    std::string getText(void) const override;
    bool dropCursor(movedir dir) override;
    bool empty(void) override;

    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // PAREN_HPP
