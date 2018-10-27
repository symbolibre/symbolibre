#ifndef FRACRENDERER_H
#define FRACRENDERER_H

#include <QWidget>
#include <EditionTree.hpp>

/* From an ideomatic point of view, the main app communicate inputs to
 * this renderer, and the renderer transmits them to the edition tree that
 * modifies itself. Yet, we could take a short cut and send the inputs directly
 * to the edition tree, but I'm not going to do that to preserve a hierarchy
 * among the system. */

enum arrowkey { LEFT, RIGHT, UP, DOWN };

class FracRenderer : public QWidget
{
    Q_OBJECT

public:
    Flow expr;

public:

    FracRenderer(QWidget *parent = 0);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void recvArrow(arrowkey dir);
    void recvDigit(int digit);
    void recvChar(char c);
    void recvLParen(void);
    void recvRParen(void);
    void recvFrac(void);
    void recvDelete(void);
    void recvClear(void);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // FRACRENDERER_H
