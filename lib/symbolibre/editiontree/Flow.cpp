// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Flow.hpp"
#include "EditionArea.hpp"
#include "Frac.hpp"
#include "Paren.hpp"
#include "Operator.hpp"
#include "Root.hpp"

#include <QDebug>
#include <QJsonArray>

#include <algorithm>
#include <cassert>

/* ****************************************************************** */
/* **********************       FLOW NODE      ********************** */
/* ****************************************************************** */

Flow::Flow(QString strinit) : EditionNode(),
    flow(), edited_node(flow.end())
{
    /* The flow is initialized with an edition area. */
    auto new_text = std::make_unique<EditionArea>(strinit);
    edited_node = flow.insert(flow.begin(), std::move(new_text));
}

Flow::Flow(Flow &&other) : EditionNode(std::move(other)),
    flow(std::move(other.flow)), edited_node(other.edited_node)
{
    other.edited_node = other.flow.end();
}

Flow &Flow::operator=(Flow &&other)
{
    flow = std::move(other.flow);
    edited_node = other.edited_node;
    other.edited_node = other.flow.end();
    return *this;
}

void Flow::ascii(int shift, bool cc)
{
    /* Flow nodes have only one child, so quite east too */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FLOW\n";
    for (auto it = flow.begin(); it != flow.end(); it++)
        (*it)->ascii(shift + 1, cc && it == edited_node);
}

QString Flow::getText(void) const
/* FIXME : Bad complexity */
{
    QString str;
    for (auto it = flow.begin(); it != flow.end(); it++)
        str.insert(str.size(), (*it)->getText());
    return str;
}

bool Flow::dropCursor(movedir dir)
{
    if (dir == MLEFT || dir == MDOWN)
        edited_node = flow.begin();
    else if (dir == MRIGHT || dir == MUP)
        edited_node = --flow.end();
    else
        return false;

    /* the extreme nodes of a flow are edition areas, so we can drop into them */
    return (*edited_node)->dropCursor(dir);
}

bool Flow::empty(void) const
{
    /* A flow has always an edition area, thus we want to know if there is
     * only one edition area, and if it is empty. */
    assert(flow.begin() != flow.end());
    return ++flow.begin() == flow.end() && flow.front()->empty();
}

bool Flow::reachedRight(void)
{
    // FIXME why no symmetry between reachedLeft and reachedRight?
    if (edited_node == flow.end())
        return true;
    else if (++edited_node == flow.end()) {
        edited_node--;
        return true;
    } else {
        edited_node--;
        return false;
    }
}

bool Flow::reachedLeft(void)
{
    return edited_node == flow.begin();
}

/* ********************** EDITIPushed METHODS ********************** */

/* About moving the cursor */

bool Flow::editMoveRight(void)
{
    /* children are a FAILURE ! >:-( */
    if (!reachedRight()) {
        if (!(*(++edited_node))->dropCursor(MLEFT))
            (*(++edited_node))->dropCursor(MLEFT);
        return true;
    } else
        return false;
}

bool Flow::editMoveLeft(void)
{
    /* children are a FAILURE again ! :-) */
    if (!reachedLeft()) {
        if (!(*(--edited_node))->dropCursor(MRIGHT))
            (*(--edited_node))->dropCursor(MRIGHT);
        return true;
    } else
        return false;
}

bool Flow::editDelete(void)
{
    // trick to allow deleting empty internal nodes from the inside
    if (auto node = dynamic_cast<InternalEditionNode *>(edited_node->get())) {
        if (node->empty())
            ++edited_node;
        else
            return true;
    }

    const auto it_area_right = edited_node;
    const auto *area_right = dynamic_cast<EditionArea *>(it_area_right->get());
    assert(area_right);
    auto right_str = area_right->getText();

    // we don't delete anything if there is nothing at the left of the cursor
    if (empty())
        return false;
    if (reachedLeft())
        return false;

    const auto it_to_remove = --edited_node;
    assert(!reachedLeft());
    auto *area_left = dynamic_cast<EditionArea *>((--edited_node)->get());
    assert(area_left);

    flow.erase(it_to_remove);

    // we merge the edition areas at the left and at the right of the removed node
    flow.erase(it_area_right);
    area_left->setCursorPos(area_left->getText().size());
    area_left->append(right_str);
    return true;
}

bool Flow::editClear(void)
{
    if (empty())
        return false;
    flow.clear();
    auto new_text = std::make_unique<EditionArea>();
    edited_node = flow.insert(flow.begin(), std::move(new_text));
    return true;
}

bool Flow::insert(EditionNode *newnode)
{
    /* since no child succeeded to insert the node, the edited node is obviously
     * an edition area, so we have to split it into two and insert between the two
     * edition areas. */

    auto target_ed_area = dynamic_cast<EditionArea *>(edited_node->get());
    assert(target_ed_area);

    if (auto newflow = dynamic_cast<Flow *>(newnode)) {
        if (newflow->flow.size() == 1) {
            target_ed_area->editString(newflow->flow.front()->getText());
            delete newflow;
            return true;
        }

        FlowIterator new_edited_node = edited_node;
        // update the edited node
        if (newflow->edited_node != newflow->flow.end() &&
                newflow->edited_node != newflow->flow.begin())
            new_edited_node = newflow->edited_node;

        // merge the left text areas
        QString right;
        target_ed_area->cutAtCursor(right);
        auto right_ed_area = dynamic_cast<EditionArea *>(newflow->flow.back().get());
        assert(right_ed_area);
        right_ed_area->append(right);

        // merge the right text areas
        const int length_before = target_ed_area->getText().size();
        auto left_ed_area = dynamic_cast<EditionArea *>(newflow->flow.front().get());
        assert(left_ed_area);
        target_ed_area->append(left_ed_area->getText());
        target_ed_area->setCursorPos(left_ed_area->getCursorPos() + length_before);
        newflow->flow.pop_front();

        // merge the flows
        flow.splice(++edited_node, std::move(newflow->flow));
        edited_node = new_edited_node;

        delete newflow;
        return true;
    }

    // Special absorbing case of fractions
    auto frac = dynamic_cast<Frac *>(newnode);
    if (frac && !target_ed_area->empty() && target_ed_area->reachedRight()) {
        QString numerator = target_ed_area->getText();
        target_ed_area->clear();
        ++edited_node;

        auto new_text = std::make_unique<EditionArea>();
        delete frac; // FIXME FIXME FIXME
        auto new_frac = std::make_unique<Frac>(numerator);
        new_frac->editMoveDown();

        edited_node = ++flow.insert(edited_node, std::move(new_frac));
        edited_node = --flow.insert(edited_node, std::move(new_text));

        return true;
    }

    /* This code could be reduced, but would be harder to understand */
    QString right_str;
    target_ed_area->cutAtCursor(right_str);
    ++edited_node;

    auto new_text = std::make_unique<EditionArea>(right_str);

    edited_node = ++flow.insert(edited_node,
                                std::unique_ptr<EditionNode>(newnode));
    edited_node = --flow.insert(edited_node, std::move(new_text));
    // First we try to drop the cursor into the new node.
    // If it is not possible (Paren...), we try the next child (an EditionArea).
    if (!(*edited_node)->dropCursor(MLEFT)) {
        ++edited_node;
        (*edited_node)->dropCursor(MLEFT);
    }

    return true;
}

bool Flow::accept(ActiveEditionNodeVisitor &v)
{
    if (getActiveChild()->accept(v))
        return true;
    return v.visit(*this);
}

EditionNode *Flow::getActiveChild(void)
{
    return edited_node->get();
}

EditionArea *Flow::getActiveNode(void)
{
    return getActiveChild()->getActiveNode();
}

void Flow::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
/* This function could seem complicated but it is not.
 * It is just taking care of those poor parentheses that cannot compute
 * their dimensions without knowledge of their surroundings. */
{
    width         = 0;
    height        = 0;
    ascent = 0;
    qreal last_height  = 0;
    qreal last_ascent = 0;

    auto it = flow.begin();
    /* Empty case: a box will be drawn */
    if (empty()) {
        (*it)->computeDimensions(painter, 0, 0); // useless

        QFontMetricsF metrics(painter.font());
        QRectF br = metrics.boundingRect(QString("□"));

        width  = br.width();
        height = std::max(metrics.height(), br.height());
        ascent = metrics.ascent();
        return;
    }

    while (it != flow.end()) {
        auto paren = dynamic_cast<Paren *>(it->get());
        if (paren && paren->getParenType() == RPAREN) {
            it--; /* go back before the parenthesis: get the dimensions of the block */
            if (it != flow.begin() && (*it)->empty())
                it --;
            qreal left_height = (*it)->height;
            qreal left_ascent = (*it)->ascent;

            it++; /* go back to the parenthesis */
            if (it != flow.end() && (*it)->empty())
                it ++;

            /* Compute parenthesis' size: only 'width' is important */
            (*it)->computeDimensions(painter, last_height, last_ascent);
            (*it)->height = left_height;
            (*it)->ascent = left_ascent;
            qreal it_width  = (*it)->width;
            qreal it_height = (*it)->height;
            qreal it_ascent = (*it)->ascent;

            if (!(*it)->empty()) {
                last_height = it_height;
                last_ascent = it_ascent;
            }

            /* Update flow box */
            width += it_width; /* only 1 parenthesis: RPAREN ) */
            height = std::max(ascent, it_ascent)
                     + std::max(height - ascent, it_height - it_ascent);
            ascent = std::max(ascent, it_ascent);

            it ++; /* pass the parenthesis */
        } else if (paren && paren->getParenType() == LPAREN) {
            /* The annoying case: rec call */
            FlowIterator new_it = it;
            struct centeredBox sub_box = parenArea(++new_it, painter);

            /* Compute parenthesis' sizes: we keep 'width' only */
            (*it)->computeDimensions(painter, last_height, last_ascent);
            (*it)->height = sub_box.height;
            (*it)->ascent = sub_box.ascent;
            if (new_it != flow.end()) { /* RPAREN (if exists) */
                (*new_it)->computeDimensions(painter, last_height, last_ascent);
                (*new_it)->height = sub_box.height;
                (*new_it)->ascent = sub_box.ascent;
            }
            qreal it_width  = (*it)->width;
            qreal it_height = (*it)->height;
            qreal it_ascent = (*it)->ascent;

            if (!(*it)->empty()) {
                last_height  = it_height;
                last_ascent = it_ascent;
            }

            /* Updating flow box */
            if (new_it != flow.end())
                width += 2 * it_width; /* 2 parenthesis */
            else
                width += 1 * it_width;
            width += sub_box.width;
            height = std::max(ascent, it_ascent)
                     + std::max(height - ascent, it_height - it_ascent);
            ascent = std::max(ascent, it_ascent);

            /* Annoying part: skip the parenthesis (and possibly the RPAREN */
            if (new_it != flow.end())
                it = ++new_it;
            else
                it = new_it;
        } else {
            /* general case: Easy */
            (*it)->computeDimensions(painter, last_height, last_ascent);
            qreal it_width  = (*it)->width;
            qreal it_height = (*it)->height;
            qreal it_ascent = (*it)->ascent;

            if (!(*it)->empty()) {
                last_height  = it_height;
                last_ascent = it_ascent;
            }

            width += it_width;
            height = std::max(ascent, it_ascent)
                     + std::max(height - ascent, it_height - it_ascent);
            ascent = std::max(ascent, it_ascent);

            it++;
        }
    }

    /* At the end the width corresponding to interspacement. */
    int nonEmpty = numberNonEmpty();
    if (nonEmpty > 0)
        width += INTERSPACE * nonEmpty;
}

void Flow::draw(qreal x, qreal y, QPainter &painter, bool cursor)
{
    EditionNode::draw(x, y, painter, cursor);

    if (empty()) {
        // draw a box, or the cursor if selected
        QRectF brect = QRectF(x, y, width, height);
        if (cursor) {
            const auto metrics = painter.fontMetrics();
            painter.drawLine(x + width / 2, y, x + width / 2, y + metrics.height());
        } else {
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, QString("□"));
        }
    }

    for (auto it = flow.begin(); it != flow.end(); it++) {
        qreal it_y = y + ascent - (*it)->ascent;
        (*it)->draw(x, it_y, painter, it == edited_node && cursor && !empty());
        x += (*it)->width;

        if (!(*it)->empty())
            x += INTERSPACE;
    }
}

centeredBox Flow::parenArea(FlowIterator &current_node, QPainter &painter)
{
    /* FIXME It is kind of cheating, but par_rect.x stands for the ascent */
    centeredBox par_box;
    par_box.height = par_box.width = par_box.ascent = 0;

    qreal last_height  = 0;
    qreal last_ascent = 0;

    while (current_node != flow.end()) {
        auto paren = dynamic_cast<Paren *>(current_node->get());
        if (paren && paren->getParenType() == RPAREN)
            break;
        else if (paren && paren->getParenType() == LPAREN)
            /* We open a new parenthesis: recursive call then adjusting the size
             * of the parenthesis */
        {
            FlowIterator new_it = current_node;
            struct centeredBox sub_box = parenArea(++new_it, painter);

            /* Compute parenthesis' sizes */
            (*current_node)->computeDimensions(painter, last_height, last_ascent);
            /* only 'width' is OK */
            /* Dirty there : adjusting size of the parenthesis 'at hands' */
            (*current_node)->height        = sub_box.height;
            (*current_node)->ascent = sub_box.ascent;
            if (new_it != flow.end()) {
                (*new_it)->computeDimensions(painter, last_height, last_ascent);
                (*new_it)->height        = sub_box.height;
                (*new_it)->ascent = sub_box.ascent;
            }
            qreal it_width  = (*current_node)->width;
            qreal it_height = last_height  = (*current_node)->height;
            qreal it_ascent = last_ascent = (*current_node)->ascent;

            /* We add the width of the parenthesis */
            /* For that we count them: new_it == flow.end() iff the right paren
             * went missing. */
            if (new_it != flow.end())
                par_box.width += 2 * it_width;
            else
                par_box.width += 1 * it_width;
            par_box.width += sub_box.width; /* Adding length of right box */
            /* Computing height & ascent: usual formulas */
            par_box.height = std::max(par_box.ascent, it_ascent)
                             + std::max(par_box.height - par_box.ascent,
                                        it_height - it_ascent);
            par_box.ascent = std::max(par_box.ascent, it_ascent);

            /* Annoying adjustments */
            if (new_it != flow.end())
                current_node = ++new_it;
            else
                current_node = new_it;
        } else { /* Default case: copy past from computeDimensions*/
            (*current_node)->computeDimensions(painter, last_height, last_ascent);
            qreal it_width  = (*current_node)->width;
            qreal it_height = last_height  = (*current_node)->height;
            qreal it_ascent = last_ascent = (*current_node)->ascent;

            par_box.width += it_width;
            par_box.height = std::max(par_box.ascent, it_ascent)
                             + std::max(par_box.height - par_box.ascent,
                                        it_height - it_ascent);
            par_box.ascent = std::max(par_box.ascent, it_ascent);

            current_node ++;
        }
    }

    return par_box;
}


int Flow::numberNonEmpty(void)
{
    int n = 0;
    for (auto it = flow.begin(); it != flow.end(); it++)
        if (!(*it)->empty())
            n++;
    return n;
}
