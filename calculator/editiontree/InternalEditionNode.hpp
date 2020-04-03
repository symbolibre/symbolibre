#ifndef INTERNALEDITIONNODE_HPP
#define INTERNALEDITIONNODE_HPP

#include "EditionNode.hpp"

#include <vector>

class Flow;

/**
 * An EditionNode that contains a fixed number of Flow children.
 */
class InternalEditionNode : public EditionNode
{
    friend QJsonObject serializeInternalNode(const InternalEditionNode &node);
    friend EditionNode *deserializeInternalNode(QJsonObject node);

public:
    InternalEditionNode() = default;

    /**
     * Returns the string to use as key during serialization.
     */
    virtual QString getNodeType() const = 0;

    bool accept(ActiveEditionNodeVisitor &v) override;

    EditionArea *getActiveNode(void) override;

    /**
    * Returns the child where the cursor is.
     * \return the non-null active child
     */
    virtual EditionNode *getActiveChild(void) = 0;

protected:
    virtual std::vector<Flow *> getChildren() = 0;
};

#endif // INTERNALEDITIONNODE_HPP
