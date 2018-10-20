#include "editiontree.h"



void EditionTree::ascii(int shift, bool)
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

bool EditionTree::editDIGIT(int)
{
    //digit = digit; /* -Wextra */
    std::cerr   << "cannot insert digit in a raw edition tree." << std::endl;
    exit(1); /* KA-BOOM */
    return false;
}

bool EditionTree::empty()
{
    return true;
}
bool EditionTree::reachedRIGHT()
{
    return false;
}
bool EditionTree::reachedLEFT()
{
    return false;
}

bool EditionTree::editMOVERIGHT()
{
    return false;
}
bool EditionTree::editMOVELEFT()
{
    return false;
}
bool EditionTree::editMOVEUP()
{
    return false;
}
bool EditionTree::editMOVEDOWN()
{
    return false;
}

bool EditionTree::editDELETE()
{
    return false;
}


/* ****************************************************************** */
/* **********************       FLOW NODE      ********************** */
/* ****************************************************************** */

FlowNode::FlowNode(nodetype arg_ntype)
{
    width = height = center_height = 0;
    cursor_pos = 0;
    between = true;

    ntype = arg_ntype;
    itcursor_pos = flow.begin();
}

void FlowNode::ascii(int shift, bool cc)
{
    /* Flow nodes have only one child, so quite east too */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FLOW NODE\n";
    if (!between)
        for (auto it = flow.begin(); it != flow.end(); it++)
            (*it)->ascii(shift + 1, cc && it == itcursor_pos);
    else
        for (auto it = flow.begin(); it != flow.end(); it++) {
            if (it == itcursor_pos) {
                for (int i = 0; i < shift + 1; i++)
                    std::cout << "  ";
                std::cout << " └▒\n";
            }
            (*it)->ascii(shift + 1, false);
        }

    return;
}

void FlowNode::drop_cursor(movedir)
{
    return;
}

bool FlowNode::empty(void)
{
    return flow.begin() == flow.end();
}

bool FlowNode::reachedRIGHT(void)
{
    /* It the cursor can be stricly moved to the right, returns 'false'
     * otherwise returns 'true'. */
    if (flow.begin() == flow.end())
        return true; /* empty flow */
    if (itcursor_pos == flow.end())
        return true; /* should not happen */
    if (++itcursor_pos == flow.end()) {
        itcursor_pos --;
        if (between || (*itcursor_pos)->ntype == TEXT)
            return true;
        else
            return false;
    }
    itcursor_pos --;
    return false;
}

bool FlowNode::reachedLEFT(void)
{
    /* It the cursor can be stricly moved to the left, returns 'false'
     * otherwise returns 'true'. */
    if (flow.begin() == flow.end())
        return false;
    if (itcursor_pos == flow.begin()) {
        if (!between || (*itcursor_pos)->ntype == TEXT)
            return true;
        else
            return false;
    }
    return false;
}

/* ********************** EDITION METHODS ********************** */

bool FlowNode::editDIGIT(int digit)
{
    /* If the cursor is at node, recursively call on it */
    /* If the cursor is between two nodes, add a new node. This is by convention
     * the case when the flow is empty. Beware that we can be between two
     * TEXT-nodes only! */
    if (between) {
        auto new_text = std::make_unique<TextNode>();
        itcursor_pos = flow.insert(itcursor_pos, std::move(new_text));
        between = false;
    }
    (*itcursor_pos)->editDIGIT(digit);
    return true;
}

/* About moving the cursor */

bool FlowNode::editMOVERIGHT(void)
{
    /* Several cases to handle ; to main possibilities :
     * - either the cursor is between two nodes
     * - or it is on a text node. */
    if (empty())
        return false;
    if (between) {
        if (!reachedRIGHT()) {
            between = false;
            itcursor_pos++;
            (*itcursor_pos)->drop_cursor(MLEFT);
            return true;
        } else if (ntype == ROOT)
            return true; /* Extreme right : does nothing */
        else
            return false; /* The cursor cannot be moved right */
    }
    /* between == false */
    else if (!(*itcursor_pos)->editMOVERIGHT()) { /* child are a FAILURE >:-( */
        if (reachedRIGHT()) {
            if (ntype == ROOT)
                (*itcursor_pos)->drop_cursor(MRIGHT);
            else
                return false;
        } else if ((*itcursor_pos)->ntype == TEXT) {
            itcursor_pos ++;
            between = false;
        } else if ((*itcursor_pos++)->ntype != TEXT) /* always tries to move on
                                                                                                * existing text */
        {
            between = true;
            itcursor_pos --;
        } else
            between = false;

        return true;
    } else
        return true;
}

bool FlowNode::editMOVELEFT(void)
{
    /* So many cases. The astuce corrector is welcome
     * to optimize the number of lines of the following. */
    if (empty())
        return false;
    if (between) {
        if (!reachedLEFT()) {
            between = false;
            (*itcursor_pos)->drop_cursor(MRIGHT);
            return true;
        } else if (ntype == ROOT)
            return true; /* ROOT is lazy */
        else
            return false;
    }
    /* between == false */
    else if (!(*itcursor_pos)->editMOVELEFT()) { /* child are failure again... :/ */
        if (reachedLEFT()) {
            if ((*itcursor_pos)->ntype == TEXT) {
                if (ntype == ROOT) {
                    (*itcursor_pos)->drop_cursor(MLEFT);
                    return true;
                } else
                    return false;
            } else /* a bit specific : between cannot point at the left of the most
                  * western node, thus we create a text node */
            {
                auto new_text = std::make_unique<TextNode>();
                itcursor_pos = flow.insert(itcursor_pos, std::move(new_text));
                between = false;
                return true;
            }
        } else if ((*itcursor_pos)->ntype != TEXT) {
            itcursor_pos --;
            if ((*itcursor_pos)->ntype == TEXT) {
                (*itcursor_pos)->drop_cursor(MRIGHT);
                between = false;
            } else
                between = true;
            return true;
        } else {
            itcursor_pos --;
            (*itcursor_pos)->drop_cursor(MRIGHT);
            between = false;
            return true;
        }
    } else
        return true;
}

/* UP and DOWN are way easier for Flow nodes */

bool FlowNode::editMOVEUP(void)
{
    if (empty() || between)
        return false;
    else
        return (*itcursor_pos)->editMOVEUP();
}

bool FlowNode::editMOVEDOWN(void)
{
    if (empty() || between)
        return false;
    else
        return (*itcursor_pos)->editMOVEDOWN();
}

bool FlowNode::editDELETE(void)
{
    if (empty())
        return false;
    else if (between) /* FIXME : tired of sub cases */
        return false;
    else
        return (*itcursor_pos)->editDELETE();
}


/* ****************************************************************** */
/* **********************       TEXT NODE      ********************** */
/* ****************************************************************** */


TextNode::TextNode(void)
{
    ntype         = TEXT;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;
    text          =   "";
}

void TextNode::ascii(int shift, bool cc)
{
    /* Text nodes have only one child so it's easy */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (cc) {
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

void TextNode::drop_cursor(movedir dir)
{
    /* I could do a switch, but the syntax is ugly on my text editor. */
    if (dir == MRIGHT || dir == MDOWN)
        cursor_pos = text.size();
    else if (dir == MLEFT  || dir == MUP)
        cursor_pos = 0;

    return;
}

bool TextNode::empty(void)
{
    return text.size() == 0;
}

bool TextNode::reachedRIGHT(void)
{
    return cursor_pos == (int) text.size();
}
bool TextNode::reachedLEFT(void)
{
    return cursor_pos == 0;
}

bool TextNode::editMOVERIGHT(void)
{
    if (reachedRIGHT())
        return false;
    else
        cursor_pos ++;
    return true; /* And... this is not ambiguous :-) */
}

bool TextNode::editMOVELEFT(void)
{
    if (reachedLEFT())
        return false;
    else
        cursor_pos --;
    return true;
}

bool TextNode::editMOVEUP(void)
{
    cursor_pos = text.size();
    return false;
}

bool TextNode::editMOVEDOWN(void)
{
    cursor_pos = 0;
    return false;
}

bool TextNode::editDELETE(void)
{
    if (cursor_pos) {
        cursor_pos--;
        text.erase(cursor_pos, 1);
        return true;
    } else
        return false; /* nothing deleted */
}

bool TextNode::editDIGIT(int digit)
{
    /* Easy node */
    text.insert(text.begin() + cursor_pos, digit + '0');
    cursor_pos++;
    return true;
}

