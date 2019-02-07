#ifndef ROOT_HPP
#define ROOT_HPP

#include "InternalEditionNode.hpp"
#include "Flow.hpp"

/**
 * Root are semantical square root nodes (√).
 *
 * They contains a single flow 'expression'.
 * The √ is centered according to 'expression' and has the same height.
 * As the size of the 'root' depends on the size of its 'expression', it holds
 * two int 'rootWidth' and 'rootHeight' that stores the size of the symbol.
 */
class Root : public InternalEditionNode
{
private:
    Flow expression;
    int rootWidth;

public:
    Root(void);  /* I'm Root */

    void ascii(int shift, bool contains_cursor) override;
    std::string getText(void) override;
    bool dropCursor(movedir dir) override;
    bool empty(void) override;

    EditionNode *getActiveChild(void) override;

    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;

};

#endif // ROOT_HPP
