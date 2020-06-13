#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "InternalEditionNode.hpp"
#include <QString>

/**
 * This class represents variable names
 *
 * When generating a giac expression with getText, parentheses are added.
 * This is needed to handle products, e.g. AB becomes (A)(B)
 */
class Variable : public EditionNode
{
protected:
    QString name;

public:
    Variable(QString name);

    const QString &getName()
    {
        return name;
    }

    void ascii(int shift, bool contains_cursor) override;
    std::string getText(void) const override;
    bool dropCursor(movedir dir) override;
    bool empty(void) const override;

    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
};

#endif // VARIABLE_HPP
