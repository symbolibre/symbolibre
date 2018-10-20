#ifndef FRACRENDERER_H
#define FRACRENDERER_H

#include <QWidget>
#include "editiontree.h"

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
    FlowNode expr;

public:

    FracRenderer(QWidget *parent = 0);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void recvARROW(arrowkey dir);
    void recvDIGIT(int digit);
    void recvDELETE(void);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // FRACRENDERER_H
