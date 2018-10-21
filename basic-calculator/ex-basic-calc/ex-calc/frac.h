#ifndef FRAC_H
#define FRAC_H

/* Frac for "fraction" are semantic nodes.
 * Frac has two specific attributes: numerator and denominator that are flows
 * in practice.
 */

#include "editiontree.h"
#include <memory>

class Flow; /* forward declaration */

class Frac : public EditionTree
{
private:
    std::unique_ptr<Flow> numerator;
    std::unique_ptr<Flow> denominator;
    bool cursor_on_top;

public:
    Frac(void);

    void ascii(int shift, bool contains_cursor) override;
    std::string get_text(void) override; /* returns a text chain that can
                                               be sent do Sage */
    void append(std::string &str) override; /* ghost one */

    bool drop_cursor(movedir dir) override; /* return 'true' if cursor can be
                                                * dropped, 'false' otherwise. */
    void cutAtCursor(std::string &cut) override;
    /* specific to cursor repositioning */

    /* Administrative : two methods that tells if the cursor has space
     * to move right/left. */
    bool empty(void) override;
    bool reachedRight(void) override;
    bool reachedLeft(void) override;

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    bool editMoveRight(void) override;
    bool editMoveLeft(void) override;
    bool editMoveUp(void) override;
    bool editMoveDown(void) override;

    /* About some delete actions */
    bool editDelete(void) override;

    bool editChar(char symbol) override;
    bool editParen(nodetype paren_type = LPAREN) override;
    bool editFrac(void) override;
};

#endif // FRAC_H
