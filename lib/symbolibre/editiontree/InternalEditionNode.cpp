// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "InternalEditionNode.hpp"
#include "ActiveEditionNodeVisitor.hpp"
#include "Flow.hpp"

InternalEditionNode::InternalEditionNode(int nchildren) : EditionNode(),
    children(nchildren), active_child_idx(0)
{

}

bool InternalEditionNode::accept(ActiveEditionNodeVisitor &v)
{
    if (getActiveChild()->accept(v))
        return true;
    return v.visit(*this);
}

EditionArea *InternalEditionNode::getActiveNode(void)
{
    return getActiveChild()->getActiveNode();
}

EditionNode *InternalEditionNode::getActiveChild(void)
{
    return &children[active_child_idx];
}

const EditionNode *InternalEditionNode::getActiveChild(void) const
{
    return &children[active_child_idx];
}
