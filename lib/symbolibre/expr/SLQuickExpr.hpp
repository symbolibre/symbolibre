#ifndef SLQUICKEXPR_HPP
#define SLQUICKEXPR_HPP

#include <QQuickPaintedItem>
#include <QTimer>

#include "../editiontree/EditionTree.hpp"


class SLQuickExpr : public QQuickPaintedItem
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
    explicit SLQuickExpr(QQuickItem *parent = nullptr);

    QString text() const
    {
        return expr.getText();
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
    void cursorPosChanged();
    void colorChanged(QColor color);

public slots:
    void insertJson(const QString &json);

    bool moveCursorLeft();
    bool moveCursorRight();
    bool moveCursorUp();
    bool moveCursorDown();
    bool deleteChar();
    bool clear();

    void updateResetCursor();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    EditionTree expr;
    QColor textColor;
    HorizontalAlignment halign;
    VerticalAlignment valign;
    bool cursorBlink;
    QTimer cursorTimer;
};

#endif // SLQUICKEXPR_HPP
