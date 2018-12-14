#ifndef EDITIONAREA_H
#define EDITIONAREA_H

#include "EditionTree.hpp"
#include <string>
#include <memory>

/* Edition areas are the only node where text can be written.
 * It is a leaf node that contains a mere string. */

class EditionArea : public EditionTree
{
protected:
    std::string text;
    int cursor_pos;

public:
    /* getText():
     * Returns the text contained by the node. */
    std::string getText(void);

    /* set_to(str):
     * Sets the text contained by the node to 'str'. */
    void set_to(const char *str);
    void set_to(std::string str);

    /* append(str):
     * Appends 'str' to the text contained by the node. */
    void append(const char *str);
    void append(std::string &str) override;

    /* cutAtCursor(cut):
     * Cuts the whole portion of text that is contained after
     * the cursor, cursor's position included, and store the cut
     * substring into 'cut'. */
    void cutAtCursor(std::string &cut);


public:

    EditionArea(void);

    /* ascii(shift, contains_cursor):
     * Print the tree structure of the node. 'shift' should be set to 0,
     * 'contains_cursor' to true if you want to track the cursor's position. */
    void ascii(int shift, bool contains_cursor) override;

    /* dropCursor(dir):
     * A function to replace the cursor according the the direction 'dir'
     * Returns 'true' if cursor can be dropped, 'false' otherwise. */
    bool dropCursor(movedir dir) override;

    /* Administrative : */
    /* empty():
     * Returns 'true' if the current node is empty (see each particular
     * node to check convention. */
    bool empty(void) override;

    /* reachedRight():
     * Tells whether or not the cursor has place to move right. */
    bool reachedRight(void) override;

    /* reachedLeft():
     * Tells whether or not the cursor has place to move left. */
    bool reachedLeft(void) override;

    /* About moving the cursor */
    bool editMoveRight(void) override; /* Move the cursor right. */
    bool editMoveLeft(void) override;  /* Move the cursor left.  */
    bool editMoveUp(void) override;
    /* Move the cursor up. Cannot be done inside an edition area so returns
     * 'false' and does nothing. */
    bool editMoveDown(void) override;
    /* Move the cursor down. Cannot be done inside an edition area so returns
     * 'false' and does nothing. */

    /* About deleting */
    /* editDelete():
     * Deletes the node or character left to the cursor. Returns 'false'
     * if nothing has been deleted. */
    bool editDelete(void) override;

    /* editClear():
     * Clears the content of the current node. */
    bool editClear(void) override;

    /* editChar(symbol):
     * Adds the specified character at the cursor's position. */
    bool editChar(char symbol) override;

    /* editOperator(achar, qchar):
     * Adds the specified operator at the cursor's position, cutting flows if
     * necessary. 'achar' will be the ascii char that will be printed by 'ascii'
     * and 'getText' like methods, and 'qstring' is the pretty string rendered in 2D.
     * Cannot be done by edition areas, so retursn 'false' and the abovue flow will
     * cut the edition area into two and will place the operator between. */
    bool editOperator(char achar, QString qstring) override;

    /* editParen(paren_type):
     * Adds a paren at the cursor's position. You should specify if this is
     * a left paren (LPAREN) or right one (RPAREN). Cannot be done by edition
     * areas, so returns 'false' and the above flow will cut the edition area
     * in two and will place a parenthesis between. */
    bool editParen(parentype paren_type = LPAREN) override;

    /* editFrac():
     * Adds a fraction at the cursor's position. Cannot be done by edition
     * areas, so returns 'false' and the above flow will cut the edition area
     * in two and will place a fraction between. */
    bool editFrac(void) override;

    /* editRoot(void):
     * Adds a square at the cursor's position. Cannot be done by edition
     * areas, so returns 'false' and the above flow will cut the edition area
     * in two and will place a root between. */
    bool editRoot(void) override;


    /* About computing dimensions */
    /* computeDimensions(painter):
     * Computes the informations necessary to draw the expression:
     * 'width', 'height', 'center_height' for all nodes in the tree.
     * linear complexity. You should give the painter that will be
     * used to call 'draw' method then. */
    void computeDimensions(QPainter &painter) override;

    /* draw(x, y, painter, cursor):
     * Draw the text with (x,y) being the top left corner of the
     * drawn expression. If 'cursor' is set to 'true', also draw a red
     * cursor at the right place. */
    void draw(int x, int y, QPainter &painter, bool cursor) override;

};

#endif // EDITIONAREA_H
