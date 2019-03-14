#ifndef CALCSHEET_HPP
#define CALCSHEET_HPP

#include <EditionTree.hpp>
#include "SLL.hpp"
#include <memory>
#include <list>
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

#define BACKSPACE 10 /* defines the size of backspace */
#define BORDERSPACE 5 /* admnistrative size between borders */
#define DEFAULT_EDITED_AREA_SIZE 23

extern QColor _SL_LIGHT_GRAY;
extern QColor _SL_DARK_GRAY;

class CalcSheet : public QQuickPaintedItem
{
    Q_OBJECT

public:
    explicit CalcSheet(QQuickItem *parent = nullptr);

    /* Painting functions */
    void paint(QPainter *painter) override;

    /* editedAreaHeight():
     * returns the height of the edited area */
    qreal editedAreaHeight(void);

    /* paintEditedArea(painter)
     * paint the edited area, borders included.
     * Dimensions have to be computed before calling this function. */
    void paintEditedArea(QPainter *painter);

    /* paintList(QPainter *painter)
     * paint the list of previously computed expressions.
     * Dimensions have to be computed before calling this function. */
    void paintList(QPainter *painter);

    /* void recomputeDimensions(QPainter *painter, (bool) hard)
     * recompute the dimensions of the stored expression tree.
     * If hard is 'false', computes those of the 'editedExpression', as well as
     * as the LAST elements of 'results'. The others remains untouched.
     * If hard is 'true', computes those of the 'editedExpression' and
     * every one of 'expressions' and 'results'.
     * SPECIAL BEHAVIOR: do not recompute 'expr/res' dim if 'res.' is empty. */
    void recomputeDimensions(QPainter *painter, bool hard);




signals:
    void expressionChanged(void);

public slots:
    Q_INVOKABLE void recvInput(int /* KeyCode::keycode */  input);

private:
    std::list<EditionTree> expressions;
    std::list<EditionTree> results;
    EditionTree editedExpression;

    int cursorPos;
};

#endif // CALCSHEET_HPP
