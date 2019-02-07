#include "Sigma.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

#define SIGMA_CURSOR_LBOUND 2
#define SIGMA_CURSOR_RBOUND 4

Sigma::Sigma(void) : InternalEditionNode(),
    lbound(), rbound(), cursor_location(SIGMA_CURSOR_RBOUND),
    head_width(0), half_sigma_height(0), sigma_width(0)
{

}

void Sigma::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "SIGMA\n";

    for (int i = 0; i < shift + 1; i++)
        std::cout << "  ";
    std::cout << " └" << (cc && cursor_location == SIGMA_CURSOR_LBOUND ? '*' : ' ')
              << "LBOUND\n";
    lbound.ascii(shift + 2, cc && (cursor_location == SIGMA_CURSOR_LBOUND));

    for (int i = 0; i < shift + 1; i++)
        std::cout << "  ";
    std::cout << " └" << (cc && cursor_location == SIGMA_CURSOR_RBOUND ? '*' : ' ')
              << "RBOUND\n";
    rbound.ascii(shift + 2, cc && (cursor_location == SIGMA_CURSOR_RBOUND));
}

std::string Sigma::getText(void)
{
    std::string str = "0";
    return str;
}

bool Sigma::dropCursor(movedir dir)
{
    if (dir == MLEFT || dir == MDOWN) {
        cursor_location = SIGMA_CURSOR_RBOUND;
        return rbound.dropCursor(MLEFT);
    } else {
        cursor_location = SIGMA_CURSOR_LBOUND;
        return lbound.dropCursor(MRIGHT);
    }
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
    else
        return &rbound;
}

void Sigma::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    lbound.computeDimensions(painter, 0, 0);
    rbound.computeDimensions(painter, 0, 0);

    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString("⎲"));
    half_sigma_height = br.height();
    sigma_width = br.width();

    head_width = std::max(std::max(sigma_width, lbound.width), rbound.width);
    width  = head_width;
    if (width == 0)
        width = metrics.width(QChar('0'));

    width += 0;
    int low_height = half_sigma_height + lbound.height;
    int up_height  = half_sigma_height + rbound.height;
    height = low_height + up_height;
    center_height = low_height;
}

void Sigma::draw(int x, int y, QPainter &painter, bool cursor)
{
    QRect brect = QRect(x, y, width, height);

    // painter.setPen(Qt::red);
    // painter.drawRect(brect);
    //
    // painter.setPen(Qt::black);



    int y_mid = y + height - center_height;

    /* Lower bound */
    int x_lbound = x + (head_width - lbound.width) / 2;
    int y_lbound = y_mid + half_sigma_height;
    lbound.draw(x_lbound, y_lbound, painter,
                cursor && (cursor_location == SIGMA_CURSOR_LBOUND));


    /* Upper bound */
    int x_rbound = x + (head_width - rbound.width) / 2;
    int y_rbound = y_mid - half_sigma_height - rbound.height;

    rbound.draw(x_rbound, y_rbound, painter,
                cursor && (cursor_location == SIGMA_CURSOR_RBOUND));

    /* Sigma */
    QRect sigmaUP   = QRect(x + (head_width - sigma_width) / 2,
                            y_mid - half_sigma_height, sigma_width, half_sigma_height);
    QRect sigmaDOWN = QRect(x + (head_width - sigma_width) / 2,
                            y_mid, sigma_width, half_sigma_height);
    //painter.drawText(x, y_mid - half_sigma_height, QString("⎲"));
    painter.drawText(sigmaUP, Qt::AlignHCenter | Qt::AlignVCenter, QString("⎲"));
    painter.drawText(sigmaDOWN, Qt::AlignHCenter | Qt::AlignVCenter, QString("⎳"));
    //painter.drawText(x, y_mid, QString("⎳"));

}
