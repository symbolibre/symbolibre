#ifndef VISITORS_HPP
#define VISITORS_HPP

#include "EditionNode.hpp"

class MoveLeftVisitor : public ActiveEditionNodeVisitor
{
public:
    virtual bool visit(EditionNode &node)
    {
        return node.editMoveLeft();
    }
};

class MoveRightVisitor : public ActiveEditionNodeVisitor
{
public:
    virtual bool visit(EditionNode &node)
    {
        return node.editMoveRight();
    }
};

class MoveUpVisitor : public ActiveEditionNodeVisitor
{
public:
    virtual bool visit(EditionNode &node)
    {
        return node.editMoveUp();
    }
};

class MoveDownVisitor : public ActiveEditionNodeVisitor
{
public:
    virtual bool visit(EditionNode &node)
    {
        return node.editMoveDown();
    }
};
#endif // VISITORS_HPP
