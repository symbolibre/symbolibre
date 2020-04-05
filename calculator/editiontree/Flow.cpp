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

Flow::Flow(std::string strinit) : EditionNode(),
    flow(), edited_node(flow.end())
{
    /* The flow is initialized with an edition area. */
    auto new_text = std::make_unique<EditionArea>();
    new_text->set_to(std::move(strinit));
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

std::string Flow::getText(void) const
/* FIXME : Bad complexity */
{
    std::string str;
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
    // this assertion is verified because the edited node is always an edition
    // area in a flow and Flow::editDelete always returns true
    assert(dynamic_cast<EditionArea *>(edited_node->get()));

    // we don't delete anything if there is nothing at the left of the cursor
    if (empty())
        return true;
    if (reachedLeft())
        return true;

    // we merge the edition areas at the left and at the right of the removed node
    edited_node = flow.erase(--edited_node);
    std::string right_str = (*edited_node)->getText();
    edited_node = --flow.erase(edited_node);
    static_cast<EditionArea *>(edited_node->get())->append(right_str);
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
    /* since no child succeeded to insert the node, the edited node is obsviously
     * an edition area, so we have to split it into two and insert between the two
     * edition areas. */

    auto ed_area = dynamic_cast<EditionArea *>(edited_node->get());
    assert(ed_area);

    if (auto f = dynamic_cast<Flow *>(newnode)) {
        if (f->flow.size() == 1) {
            ed_area->editString(f->flow.front()->getText());
            delete f;
            return true;
        }

        qDebug() << "TODO insertion of non-trivial flow";
        return false;
    }

    // Special absorbing case of fractions
    auto frac = dynamic_cast<Frac *>(newnode);
    if (frac && !ed_area->empty() && ed_area->reachedRight()) {
        std::string numerator = ed_area->getText();
        ed_area->clear();
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
    std::string right_str;
    ed_area->cutAtCursor(right_str);
    ++edited_node;

    auto new_text  = std::make_unique<EditionArea>();
    new_text->set_to(right_str);

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

void Flow::computeDimensions(QPainter &painter, int /**/, int /**/)
/* This function could seem complicated but it is not.
 * It is just taking care of those poor parentheses that cannot compute
 * their dimensions without knowledge of their surroundings. */
{
    width         = 0;
    height        = 0;
    center_height = 0;
    int last_height  = 0;
    int last_cheight = 0;

    auto it = flow.begin();
    /* Empty case: a box will be drawn */
    if (empty()) {
        (*it)->computeDimensions(painter, 0, 0); // useless

        QFontMetrics metrics = painter.fontMetrics();
        QRect br = metrics.boundingRect(QString("⃞"));

        width  = br.width();
        height = std::max(FONT_SIZE, br.height());
        center_height = height / 2; // FIXME?
        return;
    }

    while (it != flow.end()) {
        auto paren = dynamic_cast<Paren *>(it->get());
        if (paren && paren->getParenType() == RPAREN) {
            it--; /* go back before the parenthesis: get the dimensions of the block */
            if (it != flow.begin() && (*it)->empty())
                it --;
            int left_height        = (*it)->height;
            int left_center_height = (*it)->center_height;

            it++; /* go back to the parenthesis */
            if (it != flow.end() && (*it)->empty())
                it ++;

            /* Compute parenthesis' size: only 'width' is important */
            (*it)->computeDimensions(painter, last_height, last_cheight);
            (*it)->height        = left_height;
            (*it)->center_height = left_center_height;
            int it_width  = (*it)->width;
            int it_height = (*it)->height;
            int it_center = (*it)->center_height;

            if (!(*it)->empty()) {
                last_height  = it_height;
                last_cheight = it_center;
            }

            /* Update flow box */
            width += it_width; /* only 1 parenthesis: RPAREN ) */
            height = std::max(center_height, it_center)
                     + std::max(height - center_height, it_height - it_center);
            center_height = std::max(center_height, it_center);

            it ++; /* pass the parenthesis */
        } else if (paren && paren->getParenType() == LPAREN) {
            /* The annoying case: rec call */
            FlowIterator new_it = it;
            struct centeredBox sub_box = parenArea(++new_it, painter);

            /* Compute parenthesis' sizes: we keep 'width' only */
            (*it)->computeDimensions(painter, last_height, last_cheight);
            (*it)->height        = sub_box.height;
            (*it)->center_height = sub_box.center_height;
            if (new_it != flow.end()) { /* RPAREN (if exists) */
                (*new_it)->computeDimensions(painter, last_height, last_cheight);
                (*new_it)->height        = sub_box.height;
                (*new_it)->center_height = sub_box.center_height;
            }
            int it_width  = (*it)->width;
            int it_height = (*it)->height;
            int it_center = (*it)->center_height;

            if (!(*it)->empty()) {
                last_height  = it_height;
                last_cheight = it_center;
            }

            /* Updating flow box */
            if (new_it != flow.end())
                width += 2 * it_width; /* 2 parenthesis */
            else
                width += 1 * it_width;
            width += sub_box.width;
            height = std::max(center_height, it_center)
                     + std::max(height - center_height, it_height - it_center);
            center_height = std::max(center_height, it_center);

            /* Annoying part: skip the parenthesis (and possibly the RPAREN */
            if (new_it != flow.end())
                it = ++new_it;
            else
                it = new_it;
        } else {
            /* general case: Easy */
            (*it)->computeDimensions(painter, last_height, last_cheight);
            int it_width  = (*it)->width;
            int it_height = (*it)->height;
            int it_center = (*it)->center_height;

            if (!(*it)->empty()) {
                last_height  = it_height;
                last_cheight = it_center;
            }

            width += it_width;
            height = std::max(center_height, it_center)
                     + std::max(height - center_height, it_height - it_center);
            center_height = std::max(center_height, it_center);

            it++;
        }
    }

    /* At the end the width corresponding to interspacement. */
    int nonEmpty = numberNonEmpty();
    if (nonEmpty > 0)
        width += INTERSPACE * (nonEmpty - 1);
}

void Flow::draw(int x, int y, QPainter &painter, bool cursor)
{
    //QRect brect = QRect(x, y, width, height);

    //painter.setPen(Qt::red);
    //painter.drawRect(brect);

    //painter.setPen(Qt::black);

    if (empty()) {
        // draw a box, or the cursor if selected
        QRect brect = QRect(x, y, width, height);
        if (cursor) {
            painter.setPen(Qt::red);
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, QString("|"));
            painter.setPen(Qt::black);
        } else {
            painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, QString(cursor ? "|" : "⃞"));
        }
        return;
    }

    for (auto it = flow.begin(); it != flow.end(); it++) {
        int it_y = y + height - (*it)->height
                   - center_height + (*it)->center_height;
        (*it)->draw(x, it_y, painter, it == edited_node && cursor);
        x += (*it)->width;

        if (!(*it)->empty())
            x += INTERSPACE;
    }
}

centeredBox Flow::parenArea(FlowIterator &current_node, QPainter &painter)
{
    /* FIXME It is kind of cheating, but par_rect.x stands for the center_height */
    centeredBox par_box;
    par_box.height = par_box.width = par_box.center_height = 0;

    int last_height  = 0;
    int last_cheight = 0;

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
            (*current_node)->computeDimensions(painter, last_height, last_cheight);
            /* only 'width' is OK */
            /* Dirty there : adjusting size of the parenthesis 'at hands' */
            (*current_node)->height        = sub_box.height;
            (*current_node)->center_height = sub_box.center_height;
            if (new_it != flow.end()) {
                (*new_it)->computeDimensions(painter, last_height, last_cheight);
                (*new_it)->height        = sub_box.height;
                (*new_it)->center_height = sub_box.center_height;
            }
            int it_width  = (*current_node)->width;
            int it_height = last_height  = (*current_node)->height;
            int it_center = last_cheight = (*current_node)->center_height;

            /* We add the width of the parenthesis */
            /* For that we count them: new_it == flow.end() iff the right paren
             * went missing. */
            if (new_it != flow.end())
                par_box.width += 2 * it_width;
            else
                par_box.width += 1 * it_width;
            par_box.width += sub_box.width; /* Adding length of right box */
            /* Computing height & center_height: usual formulas */
            par_box.height = std::max(par_box.center_height, it_center)
                             + std::max(par_box.height - par_box.center_height,
                                        it_height - it_center);
            par_box.center_height = std::max(par_box.center_height, it_center);

            /* Annoying adjustments */
            if (new_it != flow.end())
                current_node = ++new_it;
            else
                current_node = new_it;
        } else { /* Default case: copy past from computeDimensions*/
            (*current_node)->computeDimensions(painter, last_height, last_cheight);
            int it_width  = (*current_node)->width;
            int it_height = last_height  = (*current_node)->height;
            int it_center = last_cheight = (*current_node)->center_height;

            par_box.width += it_width;
            par_box.height = std::max(par_box.center_height, it_center)
                             + std::max(par_box.height - par_box.center_height,
                                        it_height - it_center);
            par_box.center_height = std::max(par_box.center_height, it_center);

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

QPoint Flow::getCursorCoordinates(void)
{
    size_t xPos = 0;

    auto it = flow.begin();
    while (it != flow.end()) {
        if (it == edited_node)
            break;
        xPos += (*it)->width;
        if (!(*it)->empty())
            xPos += INTERSPACE;
        it++;
    }

    QPoint posInChild = (*edited_node)->getCursorCoordinates();
    size_t yPos = height - (*edited_node)->height
                  - center_height + (*edited_node)->center_height;
    xPos += posInChild.x();
    yPos += posInChild.y();
    return QPoint(xPos, yPos);
}
