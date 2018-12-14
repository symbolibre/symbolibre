#ifndef INTERNALEDITIONNODE_HPP
#define INTERNALEDITIONNODE_HPP

#include "EditionTree.hpp"

class InternalEditionNode : public EditionTree
{
public:
    bool accept(ActiveEditionNodeVisitor &v) override;

    /** Returns the leaf where the cursor is. */
    EditionTree *getActiveNode(void) override;

    virtual EditionTree *getActiveChild(void) = 0;
};

#endif // INTERNALEDITIONNODE_HPP
