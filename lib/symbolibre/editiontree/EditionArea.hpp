#ifndef EDITIONAREA_HPP
#define EDITIONAREA_HPP

#include "EditionNode.hpp"
#include <QString>
#include <memory>

/**
 * Edition areas are the only node where text can be written.
 *
 * EditionArea is a leaf node that contains a mere QString.
 * They are found in one and only one location:
 * even-numbered children of Flow nodes.
 *
 * It uses QTextLayout to compute its dimensions and the position of its cursor,
 * and QTextBoundaryFinder to determine grapheme boundaries in a Unicode text.
 */
class EditionArea : public EditionNode
{
protected:
    QString text;
    int cursor_pos;

public:
    /** Returns the text contained by the node */
    QString getText(void) const;

    /** Sets the text contained by the node to 'str' */
    void setText(QString str);

    /** Returns the position of the cursor */
    int getCursorPos(void) const;

    /** Sets the positon of the cursor */
    void setCursorPos(int pos);

    /** Appends 'str' to the text contained by the node */
    void append(QString str);

    /**
     * Cuts the whole portion of text that is contained after
     * the cursor, cursor's position included, and store the cut
     * substring into 'cut'
     */
    void cutAtCursor(QString &cut);


public:

    EditionArea(QString text = QString(), int cursor_pos = 0);

    void ascii(int shift, bool contains_cursor) override;
    bool dropCursor(movedir dir) override;
    bool empty(void) const override;
    void clear(void);
    bool reachedRight(void);
    bool reachedLeft(void);

    /* About moving the cursor */
    bool editMoveRight(void) override;
    bool editMoveLeft(void) override;
    bool editMoveUp(void) override;
    bool editMoveDown(void) override;
    bool editDelete(void) override;
    void editString(const QString &str);
    bool insert(EditionNode *newnode) override;

    EditionArea *getActiveNode(void)
    {
        return this;
    }

    void computeDimensions(QPainter &painter,
                           int lheight, int lcenterheight) override;
    void draw(int x, int y, QPainter &painter, bool cursor) override;

    QPoint getCursorCoordinates(void) override;
};

#endif // EDITIONAREA_HPP
