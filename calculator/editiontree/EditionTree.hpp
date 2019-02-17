#ifndef EDITIONTREE_HPP
#define EDITIONTREE_HPP

#include "Flow.hpp"
#include "visitors.hpp"

#include "EditionArea.hpp"
#include "Frac.hpp"
#include "Operator.hpp"
#include "Paren.hpp"
#include "Root.hpp"
#include "Power.hpp"
#include "Sigma.hpp"

/* Method calls are forwarded to the Flow member.
 * See the documentation of EditionTree for API information. */
class EditionTree
{
public:
    Flow root;

public:
    EditionTree(std::string text = "") : root(text) {}

    int getWidth() const
    {
        return root.width;
    }
    int getHeight() const
    {
        return root.height;
    }
    int getCenterHeight() const
    {
        return root.center_height;
    }

    void ascii(bool draw_cursor)
    {
        return root.ascii(0, draw_cursor);
    }

    std::string getText(void)
    {
        return root.getText();
    }

    void append(std::string str)
    {
        return root.getActiveNode()->append(std::move(str));
    }

    bool dropCursor(movedir dir)
    {
        return root.dropCursor(dir);
    }

    bool empty(void)
    {
        return root.empty();
    }

    bool editMoveRight(void)
    {
        MoveRightVisitor v;
        return root.accept(v);
    }

    bool editMoveLeft(void)
    {
        MoveLeftVisitor v;
        return root.accept(v);
    }

    bool editMoveUp(void)
    {
        MoveUpVisitor v;
        return root.accept(v);
    }

    bool editMoveDown(void)
    {
        MoveDownVisitor v;
        return root.accept(v);
    }

    bool editDelete(void)
    {
        DeleteVisitor v;
        return root.accept(v);
    }

    bool editClear(void)
    {
        return root.editClear();
    }

    void editChar(char symbol)
    {
        root.getActiveNode()->editChar(symbol);
    }

    bool editOperator(char achar, QString qs)
    {
        InsertVisitor v(new Operator(achar, qs));
        return root.accept(v);
    }

    bool editParen(parentype paren_type = LPAREN)
    {
        InsertVisitor v(new Paren(paren_type));
        return root.accept(v);
    }

    bool editFrac(void)
    {
        InsertVisitor v(new Frac);
        return root.accept(v);
    }

    bool editRoot(void)
    {
        InsertVisitor v(new Root);
        return root.accept(v);
    }

    bool editPower(void)
    {
        InsertVisitor v(new Power);
        return root.accept(v);
    }

    bool editSigma(void)
    {
        InsertVisitor v(new Sigma);
        return root.accept(v);
    }

    EditionArea *getActiveNode(void)
    {
        return root.getActiveNode();
    }

    void computeDimensions(QPainter &painter, int lheight, int lcenterheight)
    {
        return root.computeDimensions(painter, lheight, lcenterheight);
    }

    void computeDimensions(QPainter &painter)
    {
        return root.computeDimensions(painter, 0, 0);
    }

    void draw(int x, int y, QPainter &painter, bool cursor)
    {
        return root.draw(x, y, painter, cursor);
    }

    void draw(int x, int y, QPainter &painter, bool cursor, int)
    {
        return root.draw(x, y, painter, cursor);
    }

    QPoint getCursorCoordinates(void)
    {
        return root.getCursorCoordinates();
    }
};

#endif // EDITIONTREE_HPP