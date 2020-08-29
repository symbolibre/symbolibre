#ifndef EDITIONNODE_HPP
#define EDITIONNODE_HPP

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <QJsonObject>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QRect>

#include "graphicalParameters.hpp"
#include "ActiveEditionNodeVisitor.hpp"

/** \mainpage
 * \section intro A brief introduction to edition trees
 *
 * Edition Trees are a structure thought to handle the interactive edition of a
 * mathematical formula, while having enough structure to allow an on-the-run
 * beautifying of the previous in reasonable time.
 *
 * Edition Trees are classic trees with three kinds of nodes:
 * - edition areas: leaves which contain a single string.
 * - semantic nodes: these nodes are the axis to beautify the expression; these
 * are fractions, parenthesis, square roots, integrals...
 * - flows: these are infinitely long chains of edition areas and
 * semantic nodes that correspond to a line block.
 *
 * An edition tree has also a *cursor*. The cursor points exclusively to an
 * edition area that is the one being currently edited. Thus, any input will
 * modify the structure at this specific edition area.
 *
 * Here are some properties that an edition tree satisfies:
 * 1. flows begin and end with edition areas,
 * 2. flows contain nodes and edition areas, nodes contain flows, edition
 * areas are terminal,
 * 3. two semantic nodes in a flow have to be separated by an edition area,
 * 4. there cannot be two consecutive edition areas in a flow,
 * 5. the cursor always point to an edition area.
 */

enum movedir { MNONE, MRESET, MLEFT, MRIGHT, MUP, MDOWN };
enum parentype { LPAREN, RPAREN };

class ActiveEditionNodeVisitor;
class EditionArea;


/** The abstract class that serves as a base for all the nodes.
 * You should not use it directly, see the wrapper class EditionTree.
 */
class EditionNode
{
public:
    /** The width of the node */
    qreal width;
    /** The height of the node */
    qreal height;
    /** The distance from the top of the node to its baseline */
    qreal ascent;
private:
    /** The position of the node */
    QPointF mPos;

public:
    /** Constructor */
    EditionNode() : width(0), height(0), ascent(0), mPos() {}
    virtual ~EditionNode() = default;

    // FIXME needs Flow::flow to be a vector
    EditionNode(const EditionNode &) = delete;
    EditionNode(EditionNode &&) = default;

    QPointF getPos() const
    {
        return mPos;
    }

    /** Accepts a visitor. \see ActiveEditionNodeVisitor */
    virtual bool accept(ActiveEditionNodeVisitor &v)
    {
        return v.visit(*this);
    }

    /** Print the tree structure of the node.
     * \param shift Depth, initially 0.
     * \param contains_cursor true if the node or one of its children
     * contains the cursor.
     */
    virtual void ascii(int shift, bool contains_cursor) = 0;

    /** Returns a string that is python-parsable. Beware that this
     * function is sub-optimal FIXME why?
     */
    virtual QString getText(void) const = 0;

    /** A function called when the cursor enters the node oriented in the
     * direction 'dir'.
     * \return true iff cursor can be dropped.
     */
    virtual bool dropCursor(movedir dir) = 0;

    /**
     * Returns 'true' if the current node is empty.
     * By convention, a root node is empty iff its content is empty.
     */
    virtual bool empty(void) const = 0;

    /** Move the cursor right.
     * \return true if the operation succeeded,
     * false if it has to be done above in the tree.
     */
    virtual bool editMoveRight(void)
    {
        return false;
    }
    /** Move the cursor right.
     * \return true if the operation succeeded,
     * false if it has to be done above in the tree.
     */
    virtual bool editMoveLeft(void)
    {
        return false;
    }
    /** Move the cursor right.
     * \return true if the operation succeeded,
     * false if it has to be done above in the tree.
     */
    virtual bool editMoveUp(void)
    {
        return false;
    }
    /** Move the cursor right.
     * \return true if the operation succeeded,
     * false if it has to be done above in the tree.
     */
    virtual bool editMoveDown(void)
    {
        return false;
    }

    /** Deletes the node or character left to the cursor.
     * \return true iff something has been deleted.
     */
    virtual bool editDelete(void)
    {
        return false;
    }

    /** Clears the content of the innermost non-empty selected flow.
     * \return true iff something has been deleted.
     */
    virtual bool editClear(void)
    {
        return false;
    }

    /** Inserts a new node into the tree.
     * Takes ownership if succeeds.
     * \return true iff the insertion succeeded.
     */
    // FIXME leak in case it fails
    virtual bool insert(EditionNode *)
    {
        return false;
    }

    /** Returns the leaf where the cursor is.
     * The selected leaf is always an edition area.
     * \return A non-null pointer to the selected EditionArea leaf.
     */
    virtual EditionArea *getActiveNode(void)
    {
        return nullptr;
    }

    /**
     * Computes the information necessary to draw the expression.
     * It updates 'width', 'height', 'ascent' for all nodes in the
     * tree.
     * The complexity is linear in the number of nodes.
     * \param painter the painter that will be used
     * to call 'draw' method then.
     */
    virtual void computeDimensions(QPainter &painter, qreal, qreal) = 0;

    /**
     * Draw the formula.
     * \param x the left of the drawing area.
     * \param y the top of the drawing area.
     * \param cursor true to draw the cursor
     */
    virtual void draw(qreal x, qreal y, QPainter & /*painter*/,bool /*cursor*/)
    {
        mPos = QPointF(x, y);
    }
};

#endif // EDITIONNODE_HPP
