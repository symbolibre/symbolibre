#include "Sigma.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

#define SIGMA_CURSOR_BODY   1
#define SIGMA_CURSOR_LBOUND 2
#define SIGMA_CURSOR_RBOUND 4

Sigma::Sigma(void) : InternalEditionNode(),
    body(), lbound(), rbound(), cursor_location(SIGMA_CURSOR_RBOUND),
    head_width(0), half_sigma_height(0)
{

}

void Sigma::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "SIGMA\n";
    lbound.ascii(shift + 1, cc && (cursor_location == SIGMA_CURSOR_LBOUND));
    body.ascii(shift + 1, cc && (cursor_location == SIGMA_CURSOR_BODY));
    rbound.ascii(shift + 1, cc && (cursor_location == SIGMA_CURSOR_RBOUND));
}

std::string Sigma::getText(void)
{
    std::string str = "0";
    return str;
}

bool Sigma::dropCursor(movedir dir)
{
    /* Convention: the cursor is ALWAYS dropped on the body of a sugma */
    cursor_location = SIGMA_CURSOR_BODY;
    return body.dropCursor(dir);
}

bool Sigma::empty(void)
{
    return false;
}

bool Sigma::editMoveUp(void)
{
    if (cursor_location == SIGMA_CURSOR_RBOUND)
        return false;

    cursor_location = SIGMA_CURSOR_RBOUND;
    rbound.dropCursor(MLEFT);
    return true;
}

bool Sigma::editMoveDown(void)
{
    if (cursor_location == SIGMA_CURSOR_LBOUND)
        return false;

    cursor_location = SIGMA_CURSOR_LBOUND;
    lbound.dropCursor(MLEFT);
    return true;
}

EditionNode *Sigma::getActiveChild(void)
{
    if (cursor_location == SIGMA_CURSOR_LBOUND)
        return &lbound;
    if (cursor_location == SIGMA_CURSOR_RBOUND)
        return &rbound;
    else
        return &body;
}

void Sigma::computeDimensions(QPainter &painter)
{
    lbound.computeDimensions(painter);
    rbound.computeDimensions(painter);
    body.computeDimensions(painter);

    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString("⎲"));
    half_sigma_height = br.height();
    int sigma_width = br.width();

    head_width = std::max(std::max(sigma_width, lbound.width), rbound.width);
    width  = body.width + head_width;
    if (width == 0)
        width = metrics.width(QChar('0'));

    width += 0;
    int low_height = std::max(half_sigma_height + lbound.height, body.center_height);
    int up_height  = std::max(half_sigma_height + rbound.height, body.height
                              - body.center_height);
    height = low_height + up_height;
    center_height = low_height;
}

void Sigma::draw(int x, int y, QPainter &painter, bool cursor)
{
    int y_mid = y + height - center_height;

    /* Lower bound */
    int x_lbound = x + (head_width - lbound.width) / 2;
    int y_lbound = y_mid - half_sigma_height - lbound.height;
    lbound.draw(x_lbound, y_lbound, painter,
                cursor && (cursor_location == SIGMA_CURSOR_LBOUND));


    /* Upper bound */
    int x_rbound = x + (head_width - rbound.width) / 2;
    int y_rbound = y_mid + half_sigma_height;

    rbound.draw(x_rbound, y_rbound, painter,
                cursor && (cursor_location == SIGMA_CURSOR_RBOUND));

    /* Body */
    body.draw(x + head_width, y_mid - body.height + body.center_height,
              painter, cursor && (cursor_location == SIGMA_CURSOR_BODY));

    /* Sigma */
    painter.drawText(x, y_mid - half_sigma_height, QString("⎲"));
    painter.drawText(x, y_mid, QString("⎳"));

}
