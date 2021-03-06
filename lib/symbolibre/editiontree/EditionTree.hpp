// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef EDITIONTREE_HPP
#define EDITIONTREE_HPP

#ifdef HAVE_CONFIG_H
# include <giac/config.h>
#endif
#include <giac/giac.h>

#include <QJsonArray>
#include <QQuickPaintedItem>
#include <QTimer>

#include "Flow.hpp"
#include "visitors.hpp"

#include "EditionArea.hpp"
#include "Frac.hpp"
#include "Operator.hpp"
#include "Paren.hpp"
#include "Root.hpp"
#include "Power.hpp"
#include "serialization.hpp"
#include "Sigma.hpp"

/* Method calls are forwarded to the Flow member.
 * See the documentation of EditionTree for API information. */
class EditionTree
{
public:
    Flow root;

public:
    EditionTree(QString text = "") : root(text), lastEdition(0) {}
    EditionTree(const QJsonArray &json) :
        root(deserializeFlow(json)), lastEdition(0) {}

    QJsonArray serialize(bool cursor) const
    {
        return serializeFlow(root, cursor);
    }

    void deserialize(const QJsonArray &json)
    {
        root = deserializeFlow(json);
        lastEdition = 0;
    }

    qreal getWidth() const
    {
        return root.width;
    }
    qreal getHeight() const
    {
        return root.height;
    }
    qreal getAscent() const
    {
        return root.ascent;
    }

    void ascii(bool draw_cursor)
    {
        return root.ascii(0, draw_cursor);
    }

    QString getText(void) const
    {
        return root.getText();
    }

    char getLastEdition(void)
    {
        return lastEdition;
    }

    void append(QString str)
    {
        return root.getActiveNode()->append(str);
    }

    bool dropCursor(movedir dir)
    {
        return root.dropCursor(dir);
    }

    bool empty(void) const
    {
        return root.empty();
    }

    bool editMoveRight(void)
    {
        MoveRightVisitor v;
        return root.accept(v);
    }

    bool editMoveLeft(void)
    {
        MoveLeftVisitor v;
        return root.accept(v);
    }

    bool editMoveUp(void)
    {
        MoveUpVisitor v;
        return root.accept(v);
    }

    bool editMoveDown(void)
    {
        MoveDownVisitor v;
        return root.accept(v);
    }

    bool editDelete(void)
    {
        DeleteVisitor v;
        return root.accept(v);
    }

    bool editClear(void)
    {
        return root.editClear();
    }

    void editChar(char symbol)
    {
        root.getActiveNode()->editString(QString(1, symbol));
    }

    void editStr(const QString &str)
    {
        root.getActiveNode()->editString(str);
    }

    void editStr(const std::string &str)
    {
        editStr(QString::fromStdString(str));
    }

    void editStr(const char *str)
    {
        editStr(QString(str));
    }

    bool editOperator(char achar, QString qs)
    {
        lastEdition = achar;
        InsertVisitor v(new Operator(achar, qs));
        return root.accept(v);
    }

    bool editOperator(char achar, const char *qs)
    {
        lastEdition = achar;
        InsertVisitor v(new Operator(achar, QString(qs)));
        return root.accept(v);
    }

    bool editParen(parentype paren_type = LPAREN)
    {
        lastEdition = (paren_type == LPAREN) ? '(' : ')';
        InsertVisitor v(new Paren(paren_type));
        return root.accept(v);
    }

    bool editFrac(void)
    {
        lastEdition = 0;
        InsertVisitor v(new Frac);
        return root.accept(v);
    }

    bool editRoot(void)
    {
        lastEdition = 0;
        InsertVisitor v(new Root);
        return root.accept(v);
    }

    bool editPower(void)
    {
        lastEdition = '^';
        InsertVisitor v(new Power);
        return root.accept(v);
    }

    bool editSigma(void)
    {
        lastEdition = 0;
        InsertVisitor v(new Sigma);
        return root.accept(v);
    }

    void insert(EditionNode *node)
    {
        lastEdition = 0;
        InsertVisitor v(node);
        root.accept(v);
    }

    EditionArea *getActiveNode(void)
    {
        return root.getActiveNode();
    }

    void computeDimensions(QPainter &painter, qreal lheight, qreal lascent)
    {
        return root.computeDimensions(painter, lheight, lascent);
    }

    void computeDimensions(QPainter &painter)
    {
        return root.computeDimensions(painter, 0, 0);
    }

    void draw(qreal x, qreal y, QPainter &painter, bool cursor)
    {
        return root.draw(x, y, painter, cursor);
    }

    QPoint getCursorCoordinates()
    {
        return getActiveNode()->getCursorCoordinates();
    }

private:
    char lastEdition;
};

/** Make the conversion from giac to edition trees.
 * @param expr is a giac::gen corresponding to the source gen.
 * @param shell is an edition tree that will be modified accordingly.
 */
void copyExprAtCursor(giac::gen &expr, EditionTree &shell);

#endif // EDITIONTREE_HPP

