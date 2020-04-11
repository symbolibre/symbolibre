#ifndef EDITIONTREE_HPP
#define EDITIONTREE_HPP

#include <giac/config.h>
#include <giac/giac.h>

#include <QJsonArray>
#include <QQuickPaintedItem>

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
    EditionTree(std::string text = "") : root(text), lastEdition(0) {}
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

    int getWidth() const
    {
        return root.width;
    }
    int getHeight() const
    {
        return root.height;
    }
    int getCenterHeight() const
    {
        return root.center_height;
    }

    void ascii(bool draw_cursor)
    {
        return root.ascii(0, draw_cursor);
    }

    std::string getText(void) const
    {
        return root.getText();
    }

    char getLastEdition(void)
    {
        return lastEdition;
    }

    void append(std::string str)
    {
        return root.getActiveNode()->append(std::move(str));
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
        root.getActiveNode()->editString(std::string(1, symbol));
    }

    void editStr(const std::string &str)
    {
        root.getActiveNode()->editString(str);
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

    void computeDimensions(QPainter &painter, int lheight, int lcenterheight)
    {
        return root.computeDimensions(painter, lheight, lcenterheight);
    }

    void computeDimensions(QPainter &painter)
    {
        return root.computeDimensions(painter, 0, 0);
    }

    void draw(int x, int y, QPainter &painter, bool cursor)
    {
        return root.draw(x, y, painter, cursor);
    }

    QPoint getCursorCoordinates(void)
    {
        return root.getCursorCoordinates();
    }

private:
    char lastEdition;
};

/** Make the conversion from giac to edition trees.
 * @param expr is a giac::gen corresponding to the source gen.
 * @param shell is an edition tree that will be modified accordingly.
 */
void copyExprAtCursor(giac::gen &expr, EditionTree &shell);

class ETBox : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(qreal implicitWidth READ implicitWidth NOTIFY implicitWidthChanged)
    Q_PROPERTY(qreal implicitHeight READ implicitHeight NOTIFY implicitHeightChanged)
    Q_PROPERTY(QString text READ text NOTIFY exprChanged STORED false)
    Q_PROPERTY(QString json READ json WRITE setJson NOTIFY exprChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(HorizontalAlignment horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment)
    Q_PROPERTY(VerticalAlignment verticalAlignment READ verticalAlignment WRITE setVerticalAlignment)

public:
    explicit ETBox(QQuickItem *parent = nullptr);

    QString text() const
    {
        return QString::fromStdString(expr.getText());
    }

    QString json() const;
    void setJson(const QString &json);

    /* Painting functions */
    void paint(QPainter *painter) override;

    QColor color() const
    {
        return textColor;
    }
    void setColor(QColor color);

    enum HorizontalAlignment { AlignLeft, AlignRight, AlignHCenter };
    Q_ENUM(HorizontalAlignment)

    HorizontalAlignment horizontalAlignment() const
    {
        return halign;
    }

    void setHorizontalAlignment(HorizontalAlignment align);

    enum VerticalAlignment { AlignTop, AlignBottom, AlignVCenter };
    Q_ENUM(VerticalAlignment)

    VerticalAlignment verticalAlignment() const
    {
        return valign;
    }

    void setVerticalAlignment(VerticalAlignment align);

signals:
    void exprChanged();
    void colorChanged(QColor color);

public slots:
    void insertJson(const QString &json);

    bool moveCursorLeft();
    bool moveCursorRight();
    bool moveCursorUp();
    bool moveCursorDown();
    bool deleteChar();
    bool clear();

private:
    EditionTree expr;
    QColor textColor;
    HorizontalAlignment halign;
    VerticalAlignment valign;
};




#endif // EDITIONTREE_HPP

