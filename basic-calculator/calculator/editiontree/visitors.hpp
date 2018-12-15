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

class InsertVisitor : public ActiveEditionNodeVisitor
{
public:
    InsertVisitor(EditionNode *n) : newnode(n)
    {
        assert(n);
    }
    InsertVisitor(const InsertVisitor *) = delete;
    virtual bool visit(EditionNode &node)
    {
        return node.insert(newnode);
    }

private:
    EditionNode *newnode;
};

#endif // VISITORS_HPP
