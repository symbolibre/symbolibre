#ifndef INTERNALEDITIONNODE_HPP
#define INTERNALEDITIONNODE_HPP

#include "EditionNode.hpp"

/**
 * An EditionNode that contains a fixed number of Flow children.
 */
class InternalEditionNode : public EditionNode
{
public:
    bool accept(ActiveEditionNodeVisitor &v) override;

    EditionArea *getActiveNode(void) override;

    /**
    * Returns the child where the cursor is.
     * \return the non-null active child
     */
    virtual EditionNode *getActiveChild(void) = 0;
};

#endif // INTERNALEDITIONNODE_HPP
