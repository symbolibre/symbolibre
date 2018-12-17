#ifndef INTERNALEDITIONNODE_HPP
#define INTERNALEDITIONNODE_HPP

#include "EditionNode.hpp"

class InternalEditionNode : public EditionNode
{
public:
    bool accept(ActiveEditionNodeVisitor &v) override;

    EditionArea *getActiveNode(void) override;

    /** Returns the child where the cursor is. */
    virtual EditionNode *getActiveChild(void) = 0;
};

#endif // INTERNALEDITIONNODE_HPP
