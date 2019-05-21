#ifndef FLOW_HPP
#define FLOW_HPP

#include "InternalEditionNode.hpp"

/* This structure is for convenience. */
struct centeredBox {
    int width;
    int height;
    int center_height;
};

/**
 * Flows are a key part of edidion trees.
 * A Flow contains a list of non-Flow nodes.
 * \see EditionNode
 */
class Flow : public InternalEditionNode
{
    using FlowIterator = std::list<std::unique_ptr<EditionNode>>::iterator;

private:
    // FIXME get rid of std::list
    std::list<std::unique_ptr<EditionNode>> flow;
    FlowIterator edited_node;

public:
    Flow(std::string strinit = "");

    void ascii(int shift, bool contains_cursor) override;

    std::string getText(void)  override;

    bool dropCursor(movedir dir) override;

    /* Administrative: */
    bool empty(void) override;
    bool reachedRight(void);
    bool reachedLeft(void);

    /* About moving the cursor */
    bool editMoveRight(void) override; /* Move the cursor right. */
    bool editMoveLeft(void) override;  /* Move the cursor left.  */

    /* About deleting */
    bool editDelete(void) override;
    bool editClear(void) override;

    bool insert(EditionNode *newnode) override;

    EditionNode *getActiveChild(void) override;

    /* About computing dimensions */
    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;

    /** Returns the box size that the paren at cur_node-- grap. */
    centeredBox parenArea(FlowIterator &cur_node, QPainter &painter);

    /** return the number of non-empty nodes within the flow. */
    int numberNonEmpty(void);

    QPoint getCursorCoordinates(void);

};

#endif // FLOW_HPP
