#ifndef EDITIONTREE_HPP
#define EDITIONTREE_HPP

#include "Flow.hpp"
#include "visitors.hpp"

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

    void ascii(int shift, bool contains_cursor)
    {
        return root.ascii(shift, contains_cursor);
    }

    std::string getText(void)
    {
        return root.getText();
    }

    void append(std::string &str)
    {
        return root.append(str);
    }

    bool dropCursor(movedir dir)
    {
        return root.dropCursor(dir);
    }

    void cutAtCursor(std::string &cut)
    {
        return root.cutAtCursor(cut);
    }

    bool empty(void)
    {
        return root.empty();
    }

    bool reachedRight(void)
    {
        return root.reachedRight();
    }

    bool reachedLeft(void)
    {
        return root.reachedLeft();
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
        return root.editDelete();
    }

    bool editClear(void)
    {
        return root.editClear();
    }

    bool editChar(char symbol)
    {
        return root.editChar(symbol);
    }

    bool editOperator(char achar, QString qstring)
    {
        return root.editOperator(achar, qstring);
    }

    bool editParen(parentype paren_type = LPAREN)
    {
        return root.editParen(paren_type);
    }

    bool editFrac(void)
    {
        return root.editFrac();
    }

    bool editRoot(void)
    {
        return root.editRoot();
    }

    EditionNode *getActiveNode(void)
    {
        return root.getActiveNode();
    }

    void computeDimensions(QPainter &painter)
    {
        return root.computeDimensions(painter);
    }

    void draw(int x, int y, QPainter &painter, bool cursor)
    {
        return root.draw(x, y, painter, cursor);
    }
};

#endif // EDITIONTREE_HPP
