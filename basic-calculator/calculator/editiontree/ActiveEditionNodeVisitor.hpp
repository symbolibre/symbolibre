#ifndef ACTIVEEDITIONNODEVISITOR_HPP
#define ACTIVEEDITIONNODEVISITOR_HPP

class EditionNode;

/**
 * A visitor for edition trees.
 *
 * This visitor visits the active leaf, then goes up until a visit returns true.
 */
class ActiveEditionNodeVisitor
{
public:
    ActiveEditionNodeVisitor() = default;
    virtual bool visit(EditionNode &node) = 0;
};

#endif // ACTIVEEDITIONNODEVISITOR_HPP
