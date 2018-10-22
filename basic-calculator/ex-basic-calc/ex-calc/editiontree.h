#ifndef EDITIONTREE_H
#define EDITIONTREE_H

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QRect>

#define MIN_SIZE 20

/* A brief introduction to EDITION TREES
 *
 *
 * Edition Trees are a structure thought to handle the interactive edition of a
 * mathematical formula, while having enough structure to allow an on-the-run
 * beautifying of the previous in reasonable time.
 *
 * Edition Trees are classic trees with three kinds of nodes:
 * - edition areas : leaves which contain a single string.
 * - semantic nodes: these nodes are the axis to beautify the expression; these
 *                   are fractions, parenthesis, square roots, integrals...
 * - flows         : these are undefinitely long chains of edition areas and
 *                   semantic nodes that correspond to a line block.
 *
 * An edition tree has also a *cursor*. The cursor points exclusively to an
 * edition area that is the one being currently edited. Thus, any input will
 * modify the structure at this specific edition area.
 *
 * Here are some properties that an edition tree satifies:
 * (1) flows begin and end with edition areas,
 * (2) flows contain nodes and edition areas, nodes contain flows, edition
 *     areas are terminal,
 * (3) two semantic nodes in a flow have to be separated by an edition area,
 * (4) there cannot be two consecutive edition areas in a flow,
 * (5) the cursor always point to an edition area.
 *
 */

enum movedir { MNONE, MRESET, MLEFT, MRIGHT, MUP, MDOWN };
enum nodetype { NONE, ROOT, FLOW, TEXT, LPAREN, RPAREN, FRAC };

class EditionTree /* Edition tree represents a set of class, and should
                   * not be used alone. */
{
public: /* attributes */
    nodetype ntype;

    int width;
    int height;
    int center_height;

    int cursor_pos;
    //  bool cursor_node;

public: /* methods */
    virtual void ascii(int shift, bool contains_cursor) = 0;
    virtual std::string get_text(void) = 0; /* returns a text chain that can
                                               be sent do Sage */
    virtual void append(std::string &str) = 0; /* ghost one */

    virtual bool drop_cursor(movedir dir) = 0; /* return 'true' if cursor can be
                                                * dropped, 'false' otherwise. */
    virtual void cutAtCursor(std::string &cut) = 0;
    /* specific to cursor repositioning */

    /* Administrative : two methods that tells if the cursor has space
     * to move right/left. */
    virtual bool empty(void) = 0;
    virtual bool reachedRight(void) = 0;
    virtual bool reachedLeft(void) = 0;

    /* All edition methods return 'true' if they were able to do the edition
     * locally, and return 'false' if it has to be done above in the e.t.
     * The cursor is always either on a text node or between two non-text
     * nodes. */
    virtual bool editMoveRight(void) = 0;
    virtual bool editMoveLeft(void) = 0;
    virtual bool editMoveUp(void) = 0;
    virtual bool editMoveDown(void) = 0;

    /* About some delete actions */
    virtual bool editDelete(void) = 0;
    virtual bool editClear(void)  = 0;

    virtual bool editChar(char symbol) = 0;
    virtual bool editParen(nodetype paren_type = LPAREN) = 0;
    virtual bool editFrac(void) = 0;


    /* About computing dimension to print */
    virtual void compute_dimensions(QPainter &painter) = 0;
    virtual void draw(int x, int y, QPainter &painter, bool cursor) = 0;
};


#endif // EDITIONTREE_H
