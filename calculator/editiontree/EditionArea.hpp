#ifndef EDITIONAREA_HPP
#define EDITIONAREA_HPP

#include "EditionNode.hpp"
#include <string>
#include <memory>

#define EDITIONAREA_FORCE_PLACEHOLDER -1

/**
 * Edition areas are the only node where text can be written.
 * It is a leaf node that contains a mere string.
 */
class EditionArea : public EditionNode
{
protected:
    std::string text;
    int cursor_pos;

public:
    /** Returns the text contained by the node */
    std::string getText(void);

    /** Sets the text contained by the node to 'str' */
    void set_to(std::string str);

    /** Appends 'str' to the text contained by the node */
    void append(std::string str);

    /**
     * Cuts the whole portion of text that is contained after
     * the cursor, cursor's position included, and store the cut
     * substring into 'cut'
     */
    void cutAtCursor(std::string &cut);


public:

    EditionArea(void);

    void ascii(int shift, bool contains_cursor) override;
    bool dropCursor(movedir dir) override;
    bool empty(void) override;
    void clear(void);
    bool reachedRight(void);
    bool reachedLeft(void);

    /* About moving the cursor */
    bool editMoveRight(void) override;
    bool editMoveLeft(void) override;
    bool editMoveUp(void) override;
    bool editMoveDown(void) override;
    bool editDelete(void) override;
    void editChar(char symbol);

    EditionArea *getActiveNode(void)
    {
        return this;
    }

    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;
    void draw(int x, int y, QPainter &painter, bool cursor, int mod) override;

    QPoint getCursorCoordinates(void) override;
};

#endif // EDITIONAREA_HPP