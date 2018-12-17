#ifndef FLOW_H
#define FLOW_H

#include "InternalEditionNode.hpp"

/* This structure is for convenience. */
struct centeredBox {
    int width;
    int height;
    int center_height;
};

/* Flows are a key part of edidion trees.
 * A Flow contains a list of nodes (typically non-flows). Their properties
 * are described in 'editiontree.h' */

class Flow : public InternalEditionNode
{
    using FlowIterator = std::list<std::unique_ptr<EditionNode>>::iterator;

private:
    std::list<std::unique_ptr<EditionNode>> flow;
    FlowIterator edited_node;

public:
    Flow(std::string strinit = "");

    void ascii(int shift, bool contains_cursor) override;

    std::string getText(void)  override;

    void append(std::string &) override;

    /* dropCursor(dir):
     * If the flow is ROOT, catch all 'false' and try to replace the cursor
     * without returning 'false' ever. */
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
    void computeDimensions(QPainter &painter) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;

    /* parenArea(...):
     * Returns the box size that the paren at cur_node-- grap. */
    centeredBox parenArea(FlowIterator &cur_node, QPainter &painter);

    /* numberNonEmpty(void):
     * return the number of non-empty nodes within the flow. */
    int numberNonEmpty(void);

};

#endif // FLOW_H
