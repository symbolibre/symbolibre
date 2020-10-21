// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

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
    /**
    * Visit an EditionNode, starting from the active leaf.
     * \return whether the visitor needs to go up in the tree
     */
    virtual bool visit(EditionNode &node) = 0;
};

#endif // ACTIVEEDITIONNODEVISITOR_HPP
