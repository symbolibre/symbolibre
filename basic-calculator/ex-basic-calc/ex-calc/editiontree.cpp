#include "editiontree.h"



void EditionTree::ascii(int shift)
{
    /* Pure EditionTree are not supposed to be printed */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └ [RAW EditionTree]\n";
    return;
}

void EditionTree::drop_cursor(movedir)
{
    std::cerr << "cannot drop the cursor in a raw edition tree." << std::endl;
    exit(1);
}

EditionTree *EditionTree::editDIGIT(int)
{
    //digit = digit; /* -Wextra */
    std::cerr   << "cannot insert digit in a raw edition tree." << std::endl;
    exit(1); /* KA-BOOM */
    return NULL;
}

EditionTree *EditionTree::editMOVE(movedir)
{
    return NULL;
}


/* FlowNode */

FlowNode::FlowNode(nodetype arg_ntype)
{
    width = height = center_height = 0;
    cursor_pos = 0;
    between = true;

    ntype = arg_ntype;
    itcursor_pos = flow.begin();
}

void FlowNode::ascii(int shift)
{
    /* Flow nodes have only one child, so quite east too */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └ FLOW NODE\n";
    for (auto it = flow.begin(); it != flow.end(); it++)
        (*it)->ascii(shift + 1);
    return;
}

void FlowNode::drop_cursor(movedir)
{
    return;
}

EditionTree *FlowNode::editDIGIT(int digit)
{
    /* If the cursor is at node, recursively call on it */
    /* If the cursor is between two nodes, add a new node. This is by convention
     * the case when the flow is empty */
    if (between) {
        auto new_text = std::make_unique<TextNode>();
        new_text->cursor_node = new_text.get();
        itcursor_pos = flow.insert(itcursor_pos, std::move(new_text));
        between = false;
    }
    cursor_node = (*itcursor_pos)->editDIGIT(digit);
    return this;
}

EditionTree *FlowNode::editMOVE(movedir dir)
{
    /* A bit complicated : if the children returns NULL, we have to drop the cursor
     * somewhere */
    if (!flow.size()) {
        if (ntype == ROOT)
            return this;
        else
            return NULL; /* the last case should not happen */
    }

    if (!between) {
        cursor_node = (*itcursor_pos)->editMOVE(dir);
        if (cursor_node != NULL)
            return this;
    }
    /* otherwise : we have to manage a deplacement */
    std::cout << "special case" << std::endl;
    switch (dir) {
    case MLEFT:
        if (itcursor_pos != flow.begin()) { /* Usual case : there is space left */
            if (between) /* the cursor is between two existing nodes : drop it
                                            * on the left, RIGHT align */
            {
                between = false;
                cursor_node = (* itcursor_pos).get();
                (* itcursor_pos)->drop_cursor(MRIGHT);
                return this;
            }
            /* usual left */
            itcursor_pos--;
            if ((*itcursor_pos)->ntype != TEXT) {
                between = true;
                cursor_node = this;
                return this;
            } else {
                between = false;
                cursor_node = (*itcursor_pos).get();
                (* itcursor_pos)->drop_cursor(MRIGHT);
                return this;
            }
        } else if (ntype == ROOT) {
            std::cout << "check\n";
            (*itcursor_pos)->drop_cursor(MLEFT);
            cursor_node = (*itcursor_pos).get();
            return this; /* cannot go left from left side of ROOT */
        } else { /* No place left, not ROOT : just exit */
            return NULL;
        }
        break;
    case MRIGHT:
        if (itcursor_pos != flow.end()) { /* place right */
            itcursor_pos ++;
            if (between) /* the cursor is between two existing nodes : drop it
                                            * on the left, RIGHT align */
            {
                between = false;
                cursor_node = (*itcursor_pos).get();
                (* itcursor_pos)->drop_cursor(MLEFT);
                return this;
            }
            /* usual right */
            if ((*itcursor_pos)->ntype != TEXT) {
                between = true;
                cursor_node = this;
                return this;
            } else {
                between = false;
                cursor_node = (*itcursor_pos).get();
                (* itcursor_pos)->drop_cursor(MLEFT);
                return this;
            }
        } else if (ntype == ROOT) {
            itcursor_pos --;
            (*itcursor_pos)->drop_cursor(MRIGHT);
            cursor_node = (*itcursor_pos).get();
            return this;
        }
        /* cannot go right from right side of ROOT */
        else { /* No place right, not ROOT : just exit */
            itcursor_pos --;
            return NULL;
        }
        break;
    case MUP:
        // TODO
        break;
    case MDOWN:
        // TODO
        break;
    default:
        return this;
    }
    return this;
}

/* TextNode */

TextNode::TextNode(void)
{
    ntype         = TEXT;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;
    cursor_node   = NULL;
    text          =   "";
}

void TextNode::ascii(int shift)
{
    /* Text nodes have only one child so it's easy */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (cursor_node == this) {
        std::cout << " └ \"";
        for (int i = 0; i < cursor_pos; i++)
            std::cout << text[i];
        std::cout << "▒" ;
        for (int i = cursor_pos; i < (int)text.size(); i++)
            std::cout << text[i];
        std::cout << "\"\n";
    } else
        std::cout << " └ \"" << text << "\"\n";

    return;
}

void TextNode::drop_cursor(movedir)
{

}

EditionTree *TextNode::editDIGIT(int digit)
{
    /* Easy node */
    text.insert(text.begin() + cursor_pos, digit + '0');
    cursor_pos++;
    return this;
}

EditionTree *TextNode::editMOVE(movedir dir)
{
    /* Because text nodes are leafs, there is no need to handle recursive things */
    switch (dir) {
    case MLEFT:
        if (cursor_pos > 0) {
            cursor_pos --;
            return this;
        } else
            return cursor_node = NULL;
        break;
    case MRIGHT:
        if (cursor_pos < (int)text.size()) {
            cursor_pos ++;
            return this;
        } else
            return cursor_node = NULL;
        break;
    case MUP:
        return cursor_node = NULL;
        break;
    case MDOWN:
        return cursor_node = NULL;
        break;
    default:
        return this;
        break;
    }
    return this;
}
