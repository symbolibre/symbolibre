#ifndef MATHRENDERER_HPP
#define MATHRENDERER_HPP

#include <EditionTree.hpp>
#include "keycode.hpp"
#include "interface.hpp"

/* Class MathRenderer
 *
 * This class is a QObject that can be used to built and draw mathematical expressions.
 * Mathematical expressions edition is dealt with the library "EditionTree.hpp". The
 * expression is modified as the corrects signal are sent, and the expression is drawn
 * whenever the method "renderExpression" is called.
 *
 */

#include <QQuickPaintedItem>
#include <QDebug>

class MathRenderer : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit MathRenderer(QQuickItem *parent = nullptr);
    void paint(QPainter *painter);

signals:
    void expressionChanged(void);

public slots:
    Q_INVOKABLE void recvInput(int /* KeyCode::keycode */  input);

private:
    Flow expression;

};

#endif // MATHRENDERER_HPP
