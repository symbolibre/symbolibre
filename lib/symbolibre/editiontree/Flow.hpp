// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef FLOW_HPP
#define FLOW_HPP

#include "EditionNode.hpp"

/* This structure is for convenience. */
struct centeredBox {
    qreal width;
    qreal height;
    qreal ascent;
};

/**
 * Flows are a key part of edidion trees.
 * A Flow contains a list of non-Flow nodes.
 * \see EditionNode
 */
class Flow : public EditionNode
{
    using FlowIterator = std::list<std::unique_ptr<EditionNode>>::iterator;

    friend Flow deserializeFlow(const QJsonArray &json);
    friend QJsonArray serializeFlow(const Flow &flow, bool cursor);

private:
    // FIXME get rid of std::list
    std::list<std::unique_ptr<EditionNode>> flow;
    FlowIterator edited_node;

public:
    Flow(QString strinit = "");
    Flow(Flow &&other);
    Flow &operator=(Flow &&other);

    void ascii(int shift, bool contains_cursor) override;

    QString getText(void) const override;

    bool dropCursor(movedir dir) override;

    /* Administrative: */
    bool empty(void) const override;
    bool reachedRight(void);
    bool reachedLeft(void);

    /* About moving the cursor */
    bool editMoveRight(void) override; /* Move the cursor right. */
    bool editMoveLeft(void) override;  /* Move the cursor left.  */

    /* About deleting */
    bool editDelete(void) override;
    bool editClear(void) override;

    bool insert(EditionNode *newnode) override;

    bool accept(ActiveEditionNodeVisitor &v) override;

    EditionArea *getActiveNode(void) override;
    EditionNode *getActiveChild(void);

    /* About computing dimensions */
    void computeDimensions(QPainter &painter,
                           qreal lheight, qreal lascent) override;
    void draw(qreal x, qreal y, QPainter &painter, bool cursor) override;

    /** Returns the box size that the paren at cur_node-- grap. */
    centeredBox parenArea(FlowIterator &cur_node, QPainter &painter);

    /** return the number of non-empty nodes within the flow. */
    int numberNonEmpty(void);

};

#endif // FLOW_HPP
