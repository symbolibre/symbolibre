// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

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
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(HorizontalAlignment horizontalAlignment READ horizontalAlignment WRITE setHorizontalAlignment)
    Q_PROPERTY(VerticalAlignment verticalAlignment READ verticalAlignment WRITE setVerticalAlignment)

public:
    explicit SLQuickExpr(QQuickItem *parent = nullptr);

    QString text() const
    {
        return m_expr.getText();
    }

    QString json() const;
    void setJson(const QString &json);

    /* Painting functions */
    void paint(QPainter *painter) override;

    const QFont &font() const
    {
        return m_font;
    }
    void setFont(const QFont &font);

    QColor color() const
    {
        return m_textColor;
    }
    void setColor(QColor color);

    enum HorizontalAlignment { AlignLeft, AlignRight, AlignHCenter };
    Q_ENUM(HorizontalAlignment)

    HorizontalAlignment horizontalAlignment() const
    {
        return m_halign;
    }

    void setHorizontalAlignment(HorizontalAlignment align);

    enum VerticalAlignment { AlignTop, AlignBottom, AlignVCenter };
    Q_ENUM(VerticalAlignment)

    VerticalAlignment verticalAlignment() const
    {
        return m_valign;
    }

    void setVerticalAlignment(VerticalAlignment align);

signals:
    void exprChanged();
    void cursorPosChanged();
    void colorChanged(QColor color);
    void fontChanged(const QFont &font);

public slots:
    void insertJson(const QString &json);

    bool dropCursorLeft();
    bool dropCursorRight();
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
    EditionTree m_expr;
    QFont m_font;
    QColor m_textColor;
    HorizontalAlignment m_halign;
    VerticalAlignment m_valign;
    bool m_cursorBlink;
    QTimer m_cursorTimer;
};

#endif // SLQUICKEXPR_HPP
