#ifndef EDITIONAREA_H
#define EDITIONAREA_H

#include "editiontree.h"

class EditionArea : public EditionTree
{
protected:
    std::string text;

public:
    EditionArea();
    void ascii(int shift, bool contains_cursor) override;
    void drop_cursor(movedir dir)               override;

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
    bool editDelete(void) override;

    bool editDigit(int digit) override;
};

#endif // EDITIONAREA_H
