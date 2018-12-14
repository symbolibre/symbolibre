#include "Flow.hpp"
#include "EditionArea.hpp"
#include "Frac.hpp"
#include "Paren.hpp"
#include "Operator.hpp"
#include "Root.hpp"

#include <algorithm>

/* ****************************************************************** */
/* **********************       FLOW NODE      ********************** */
/* ****************************************************************** */

Flow::Flow(std::string strinit) : InternalEditionNode(),
    flow(), edited_node(flow.end())
{
    /* The flow is initialized with an edition area. */
    auto new_text = std::make_unique<EditionArea>();
    new_text->set_to(std::move(strinit));
    edited_node = flow.insert(flow.begin(), std::move(new_text));
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

std::string Flow::getText(void)
/* FIXME : Bad complexity */
{
    std::string str;
    for (auto it = flow.begin(); it != flow.end(); it++)
        str.insert(str.size(), (*it)->getText());
    return str;
}

void Flow::append(std::string &)
{
    return;
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

void Flow::cutAtCursor(std::string &)
{
    return;
}

bool Flow::empty(void)
{
    /* A flow has always an edition area, thus we want to know if there is
     * only one edition area, and if it is empty. */
    if (flow.begin() == flow.end())
        return true;
    return ++flow.begin() == flow.end() && (*(flow.begin()))->empty();
}

bool Flow::reachedRight(void)
{
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

bool Flow::editChar(char symbol)
{
    (*edited_node)->editChar(symbol);
    return true;
}

/* About moving the cursor */

bool Flow::editMoveRight(void)
{
    if ((*edited_node)->editMoveRight())
        return true;
    /* children are a FAILURE ! >:-( */
    else if (!reachedRight()) {
        if (!(*(++edited_node))->dropCursor(MLEFT))
            (*(++edited_node))->dropCursor(MLEFT);
        return true;
    } else
        return false;
}

bool Flow::editMoveLeft(void)
{
    if ((*edited_node)->editMoveLeft())
        return true;
    /* children are a FAILURE again ! :-) */
    else if (!reachedLeft()) {
        if (!(*(--edited_node))->dropCursor(MRIGHT))
            (*(--edited_node))->dropCursor(MRIGHT);
        return true;
    } else
        return false;
}

/* UP and DOWN are way easier for Flow nodes */

bool Flow::editMoveUp(void)
{
    if (empty())
        return false;
    else
        return (*edited_node)->editMoveUp();
}

bool Flow::editMoveDown(void)
{
    if (empty())
        return false;
    else
        return (*edited_node)->editMoveDown();
}

bool Flow::editDelete(void)
{
    if (empty())
        return false;
    if (!(*edited_node)->editDelete())
        /* child deleted nothing and a node have to be deleted*/
    {
        if (reachedLeft())
            return false;

        edited_node = flow.erase(--edited_node);
        std::string right_str = (*edited_node)->getText();
        edited_node = --flow.erase(edited_node);
        (*edited_node)->append(right_str);
        return true;
    } else
        return true; /* nothing deleted */
}

bool Flow::editClear(void)
{
    flow.clear();
    auto new_text = std::make_unique<EditionArea>();
    edited_node = flow.insert(flow.begin(), std::move(new_text));
    return true;
}

bool Flow::editOperator(char achar, QString qstring)
{
    if ((*edited_node)->editOperator(achar, qstring))
        return true;

    /* else : the edited node is obsiously an edition area so we have to
     * split it into two an insert the operator between the two fresh
     * edition areas. */

    std::string right_str;
    (*edited_node)->cutAtCursor(right_str);
    ++edited_node;

    auto new_op   = std::make_unique<Operator>(achar, qstring);
    auto new_text = std::make_unique<EditionArea>();
    new_text->set_to(right_str);

    edited_node = ++flow.insert(edited_node, std::move(new_op));
    edited_node = flow.insert(edited_node, std::move(new_text));
    (*edited_node)->dropCursor(MLEFT);

    return true;
}

bool Flow::editParen(parentype paren_type)
{
    if ((*edited_node)->editParen(paren_type))
        return true;

    /* else : the edited node is obsviously an edition area, so we have to
     * split into two an insert a parenthesis between the two edition areas. */

    /* This code could be reduced, but would be harder to understand */
    std::string right_str;
    (*edited_node)->cutAtCursor(right_str);
    ++edited_node;

    auto new_paren = std::make_unique<Paren>(paren_type);
    auto new_text  = std::make_unique<EditionArea>();
    new_text->set_to(right_str);

    edited_node = ++flow.insert(edited_node, std::move(new_paren));
    edited_node = flow.insert(edited_node, std::move(new_text));
    (*edited_node)->dropCursor(MLEFT);

    return true;
}

bool Flow::editFrac(void)
{
    if ((*edited_node)->editFrac())
        return true;

    /* else : the edited node is obsviously an edition area, so we have two
     *        cases:
     *        - either the cursor is at the end of the edition area, so
     *          the fraction absorbs it.
     *        - in the other case, we split into two and insert a fraction
     *          between the two edition areas. */

    /* This code could be reduced, but would be harder to understand */
    if (!(*edited_node)->empty() && (*edited_node)->reachedRight()) { // absorbing case.
        std::string   numerator = (*edited_node)->getText();
        (*edited_node)->editClear();
        ++edited_node;

        auto new_text = std::make_unique<EditionArea>();
        auto new_frac = std::make_unique<Frac>(numerator);

        edited_node = ++flow.insert(edited_node, std::move(new_frac));
        edited_node = --flow.insert(edited_node, std::move(new_text));

        (*edited_node)->dropCursor(MDOWN);
        editMoveDown();
    } else { // cutting case
        std::string right_str;
        (*edited_node)->cutAtCursor(right_str);
        ++edited_node;

        auto new_frac = std::make_unique<Frac>();
        auto new_text = std::make_unique<EditionArea>();
        new_text->set_to(right_str);

        edited_node = ++flow.insert(edited_node, std::move(new_frac));
        edited_node = --flow.insert(edited_node, std::move(new_text));
        (*edited_node)->dropCursor(MLEFT);
    }

    return true;
}

bool Flow::editRoot(void)
{
    if ((*edited_node)->editRoot())
        return true;

    /* else : the edited node is obsviously an edition area, so we have
     * to split it into two and to insert a root between the
     * two edition areas. */

    std::string right_str;
    (*edited_node)->cutAtCursor(right_str);
    ++edited_node;

    auto new_frac = std::make_unique<Root>();
    auto new_text = std::make_unique<EditionArea>();
    new_text->set_to(right_str);

    edited_node = ++flow.insert(edited_node, std::move(new_frac));
    edited_node = --flow.insert(edited_node, std::move(new_text));
    (*edited_node)->dropCursor(MLEFT);

    return true;
}

EditionTree *Flow::getActiveChild(void)
{
    return edited_node->get();
}

void Flow::computeDimensions(QPainter &painter)
/* This function could seems complicated but it is not.
 * It is just taking care of those poor parenthesis that cannot compute
 * their dimensions without knowing of their surroundings. */
{
    width         = 0;
    height        = 0;
    center_height = 0;

    auto it = flow.begin();
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

            /* Compute parenthesis' sizes: only 'width' is important */
            (*it)->computeDimensions(painter);
            (*it)->height        = left_height;
            (*it)->center_height = left_center_height;
            int it_width  = (*it)->width;
            int it_height = (*it)->height;
            int it_center = (*it)->center_height;

            /* Update flow box */
            width += it_width; /* only 1 parenthesis : RPAREN ) */
            height = std::max(center_height, it_center)
                     + std::max(height - center_height, it_height - it_center);
            center_height = std::max(center_height, it_center);

            it ++; /* pass the parenthesis */
        } else if (paren && paren->getParenType() == LPAREN) {
            /* The annoying case: rec call */
            FlowIterator new_it = it;
            struct centeredBox sub_box = parenArea(++new_it, painter);

            /* Compute parenthesis' sizes: we keep 'width' only */
            (*it)->computeDimensions(painter);
            (*it)->height        = sub_box.height;
            (*it)->center_height = sub_box.center_height;
            if (new_it != flow.end()) { /* RPAREN (if exists) */
                (*new_it)->computeDimensions(painter);
                (*new_it)->height        = sub_box.height;
                (*new_it)->center_height = sub_box.center_height;
            }
            int it_width  = (*it)->width;
            int it_height = (*it)->height;
            int it_center = (*it)->center_height;

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
            (*it)->computeDimensions(painter); // FIXME : parenthesis
            int it_width  = (*it)->width;
            int it_height = (*it)->height;
            int it_center = (*it)->center_height;

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
    /* It is kind of cheating, but par_rect.x stands for  the center_height */
    centeredBox par_box;
    par_box.height = par_box.width = par_box.center_height = 0;
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
            (*current_node)->computeDimensions(painter); /* only 'width' is OK */
            /* Dirty there : adjusting size of the parenthesis 'at hands' */
            (*current_node)->height        = sub_box.height;
            (*current_node)->center_height = sub_box.center_height;
            if (new_it != flow.end()) {
                (*new_it)->computeDimensions(painter);
                (*new_it)->height        = sub_box.height;
                (*new_it)->center_height = sub_box.center_height;
            }
            int it_width  = (*current_node)->width;
            int it_height = (*current_node)->height;
            int it_center = (*current_node)->center_height;

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
            (*current_node)->computeDimensions(painter);
            int it_width  = (*current_node)->width;
            int it_height = (*current_node)->height;
            int it_center = (*current_node)->center_height;

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
