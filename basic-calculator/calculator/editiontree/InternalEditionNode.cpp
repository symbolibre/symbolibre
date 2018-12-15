#include "InternalEditionNode.hpp"
#include "ActiveEditionNodeVisitor.hpp"

bool InternalEditionNode::accept(ActiveEditionNodeVisitor &v)
{
    if (getActiveChild()->accept(v))
        return true;
    return v.visit(*this);
}

EditionNode *InternalEditionNode::getActiveNode(void)
{
    return getActiveChild()->getActiveNode();
}
