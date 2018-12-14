#ifndef ACTIVEEDITIONNODEVISITOR_HPP
#define ACTIVEEDITIONNODEVISITOR_HPP

class EditionTree;

/**
 * A visitor for edition trees.
 *
 * This visitor visits the active leaf, then goes up until a visit returns true.
 */
class ActiveEditionNodeVisitor
{
public:
    ActiveEditionNodeVisitor() = default;
    virtual bool visit(EditionTree &node) = 0;
};

#endif // ACTIVEEDITIONNODEVISITOR_HPP
