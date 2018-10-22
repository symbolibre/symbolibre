#include "flow.h"
#include "editionarea.h"
#include "frac.h"
#include "paren.h"

#include <algorithm>

/* ****************************************************************** */
/* **********************       FLOW NODE      ********************** */
/* ****************************************************************** */

Flow::Flow(nodetype arg_ntype)
{
    width = height = center_height = 0;
    cursor_pos = 0; /* not used */

    ntype = arg_ntype;

    /* The flow is initialized with an edition area. */
    auto new_text = std::make_unique<EditionArea>();
    edited_node = flow.insert(flow.begin(), std::move(new_text));
    return;
}

void Flow::ascii(int shift, bool cc)
{
    /* Flow nodes have only one child, so quite east too */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FLOW\n";
    for (auto it = flow.begin(); it != flow.end(); it++)
        (*it)->ascii(shift + 1, cc && it == edited_node);
    return;
}

std::string Flow::get_text(void)
/* FIXME : Bad complexity */
{
    std::string str;
    for (auto it = flow.begin(); it != flow.end(); it++)
        str.insert(str.size(), (*it)->get_text());
    return str;
}

void Flow::append(std::string &)
{
    return;
}

bool Flow::drop_cursor(movedir dir)
{
    if (dir == MLEFT || dir == MDOWN)
        edited_node = flow.begin();
    else if (dir == MRIGHT || dir == MUP)
        edited_node = --flow.end();
    else
        return false;

    /* the extreme nodes of a flow are edition areas, so we can drop into them */
    return (*edited_node)->drop_cursor(dir);
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

/* ********************** EDITION METHODS ********************** */

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
        if (!(*(++edited_node))->drop_cursor(MLEFT))
            (*(++edited_node))->drop_cursor(MLEFT);
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
        if (!(*(--edited_node))->drop_cursor(MRIGHT))
            (*(--edited_node))->drop_cursor(MRIGHT);
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
        std::string right_str = (*edited_node)->get_text();
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


bool Flow::editParen(nodetype paren_type)
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
    (*edited_node)->drop_cursor(MLEFT);

    return true;
}

bool Flow::editFrac(void)
{
    if ((*edited_node)->editFrac())
        return true;

    /* else : the edited node is obsviously an edition area, so we have to
     * split into two an insert a fraction between the two edition areas. */

    /* This code could be reduced, but would be harder to understand */
    std::string right_str;
    (*edited_node)->cutAtCursor(right_str);
    ++edited_node;

    auto new_frac = std::make_unique<Frac>();
    auto new_text = std::make_unique<EditionArea>();
    new_text->set_to(right_str);

    edited_node = ++flow.insert(edited_node, std::move(new_frac));
    edited_node = --flow.insert(edited_node, std::move(new_text));
    (*edited_node)->drop_cursor(MLEFT);

    return true;
}

void Flow::compute_dimensions(QPainter &painter)
{
    width         = 0;
    height        = 0;
    center_height = 0;

    auto it = flow.begin();
    while (it != flow.end()) {
        if ((*it)->ntype == RPAREN) {
            it--; /* go back before the parenthesis: get the dimensions of the block */
            if (it != flow.begin() && (*it)->empty())
                it --;
            int left_height        = (*it)->height;
            int left_center_height = (*it)->center_height;

            it++; /* go back to the parenthesis */
            if (it != flow.end() && (*it)->empty())
                it ++;

            /* Compute parenthesis' sizes: only 'width' is important */
            (*it)->compute_dimensions(painter);
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

            /* Reset font size */
            it ++; /* pass the parenthesis */
        } else if ((*it)->ntype == LPAREN) {
            /* The annoying case: rec call */
            std::list<std::unique_ptr<EditionTree>>::iterator new_it = it;
            struct centeredBox sub_box = parenArea(++new_it, painter);

            /* Compute parenthesis' sizes: we keep 'width' only */
            (*it)->compute_dimensions(painter);
            (*it)->height        = sub_box.height;
            (*it)->center_height = sub_box.center_height;
            if (new_it != flow.end()) { /* RPAREN (if exists) */
                (*new_it)->compute_dimensions(painter);
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
            (*it)->compute_dimensions(painter); // FIXME : parenthesis
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
    return;
}

void Flow::draw(int x, int y, QPainter &painter, bool cursor)
{
    QRect brect = QRect(x, y, width, height);

    painter.setPen(Qt::red);
    //painter.drawRect(brect);

    painter.setPen(Qt::black);
    for (auto it = flow.begin(); it != flow.end(); it++) {
        int it_y = y + height - (*it)->height
                   - center_height + (*it)->center_height;
        (*it)->draw(x, it_y, painter, it == edited_node && cursor);
        x += (*it)->width;
    }

    return;
}

struct centeredBox
Flow::parenArea(std::list<std::unique_ptr<EditionTree>>::iterator &
                current_node, QPainter &painter)
{
    /* It is kind of cheating, but par_rect.x stands for  the center_height */
    centeredBox par_box;
    par_box.height = par_box.width = par_box.center_height = 0;
    while (current_node != flow.end()) {
        if ((*current_node)->ntype == RPAREN)
            break;
        else if ((*current_node)->ntype == LPAREN)
            /* We open a new parenthesis: recursive call then adjusting the size
             * of the parenthesis */
        {
            std::list<std::unique_ptr<EditionTree>>::iterator new_it = current_node;
            struct centeredBox sub_box = parenArea(++new_it, painter);

            /* Compute parenthesis' sizes */
            (*current_node)->compute_dimensions(painter); /* only 'width' is OK */
            /* Dirty there : adjusting size of the parenthesis 'at hands' */
            (*current_node)->height        = sub_box.height;
            (*current_node)->center_height = sub_box.center_height;
            if (new_it != flow.end()) {
                (*new_it)->compute_dimensions(painter);
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
        } else { /* Default case */
            (*current_node)->compute_dimensions(painter);
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

