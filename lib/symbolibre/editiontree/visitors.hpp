// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef VISITORS_HPP
#define VISITORS_HPP

#include <cassert>
#include "EditionNode.hpp"

class MoveLeftVisitor : public ActiveEditionNodeVisitor
{
public:
    bool visit(EditionNode &node) override
    {
        return node.editMoveLeft();
    }
};

class MoveRightVisitor : public ActiveEditionNodeVisitor
{
public:
    bool visit(EditionNode &node) override
    {
        return node.editMoveRight();
    }
};

class MoveUpVisitor : public ActiveEditionNodeVisitor
{
public:
    bool visit(EditionNode &node) override
    {
        return node.editMoveUp();
    }
};

class MoveDownVisitor : public ActiveEditionNodeVisitor
{
public:
    bool visit(EditionNode &node) override
    {
        return node.editMoveDown();
    }
};

class DeleteVisitor : public ActiveEditionNodeVisitor
{
public:
    bool visit(EditionNode &node) override
    {
        return node.editDelete();
    }
};

class ClearVisitor : public ActiveEditionNodeVisitor
{
public:
    bool visit(EditionNode &node) override
    {
        return node.editClear();
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
    bool visit(EditionNode &node) override
    {
        return node.insert(newnode);
    }

private:
    EditionNode *newnode;
};

#endif // VISITORS_HPP
