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
    friend QJsonObject serializeInternalNode(const InternalEditionNode &node, bool cursor);
    friend EditionNode *deserializeInternalNode(QJsonObject node);

public:
    InternalEditionNode(int nchildren);

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
    EditionNode *getActiveChild(void);
    const EditionNode *getActiveChild(void) const;

protected:
    std::vector<Flow> children;
    int active_child_idx;
};

#endif // INTERNALEDITIONNODE_HPP
