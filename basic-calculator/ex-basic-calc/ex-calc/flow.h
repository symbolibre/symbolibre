#ifndef FLOW_H
#define FLOW_H

#include "editiontree.h"

class Flow : public EditionTree
{
private:
    std::list<std::unique_ptr<EditionTree>> flow;
    std::list<std::unique_ptr<EditionTree>>::iterator edited_node;

public:
    Flow(nodetype arg_ntype = FLOW);
    void ascii(int shift, bool contains_cursor) override;
    std::string get_text(void)                  override;
    void append(std::string &)                   override;
    bool drop_cursor(movedir dir)               override;
    void cutAtCursor(std::string &cut)          override;

    /* Administrative : */
    bool empty(void) override;
    bool reachedRight(void) override;
    bool reachedLeft(void) override;

    /* About moving the cursor */
    bool editMoveRight(void) override;
    bool editMoveLeft(void) override;
    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    /* About deleting */
    bool editDelete(void)     override;

    /* The usual edition part */
    bool editChar(char symbol) override;
    bool editParen(nodetype paren_type = LPAREN) override;
    bool editFrac(void) override;
};

#endif // FLOW_H
