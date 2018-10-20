#include "flow.h"

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

void Flow::drop_cursor(movedir)
{
    /* TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO */
    return;
}

bool Flow::empty(void)
{
    return flow.begin() == flow.end();
}

bool Flow::reachedRight(void)
{
    if (edited_node == flow.end())
        return true;
    else if (++edited_node == flow.end()) {
        edited_node--;
        return true;
    } else
        return false;
}

bool Flow::reachedLeft(void)
{
    return edited_node == flow.begin();
}

/* ********************** EDITION METHODS ********************** */

bool Flow::editDigit(int digit)
{
    (*edited_node)->editDigit(digit);
    return true;
}

/* About moving the cursor */

bool Flow::editMoveRight(void)
{
    if ((*edited_node)->editMoveRight())
        return true;
    /* children are a FAILURE ! >:-( */
    else if (!reachedRight()) {
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
    /* FIXME : catch the value of the child, manage node deleting */
    else
        return (*edited_node)->editDelete();
}

