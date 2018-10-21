#include "flow.h"
#include "editionarea.h"
#include "frac.h"
#include "paren.h"

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

