#ifndef INTERNALEDITIONNODE_HPP
#define INTERNALEDITIONNODE_HPP

#include "EditionNode.hpp"

class InternalEditionNode : public EditionNode
{
public:
    bool accept(ActiveEditionNodeVisitor &v) override;

    /** Returns the leaf where the cursor is. */
    EditionNode *getActiveNode(void) override;

    virtual EditionNode *getActiveChild(void) = 0;
};

#endif // INTERNALEDITIONNODE_HPP
