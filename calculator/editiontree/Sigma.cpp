#include "Sigma.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

#define SIGMA_CURSOR_LBOUND 2
#define SIGMA_CURSOR_RBOUND 4

Sigma::Sigma(void) : InternalEditionNode(),
    lbound(), rbound(), cursor_location(SIGMA_CURSOR_RBOUND),
    sigma_height(0), sigma_width(0)
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
    std::string str = "sum(";
    str.insert(str.size(), lbound.getText());
    str.insert(str.size(), ",");
    str.insert(str.size(), rbound.getText());
    str.insert(str.size(), ")");
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

    //QFont font = QFont("dejavu sans mono");
    //font.setStyleHint(QFont::Monospace);
    //font.setHintingPreference(QFont::PreferFullHinting);
    QFont font = painter.font();
    font.setPixelSize(FONT_SIZE * 2);
    font.setFamily("dejavu math tex gyre");
    painter.setFont(font);

    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString("∑"));
    sigma_height = br.height();
    font.setPixelSize(FONT_SIZE);
    sigma_width = br.width();

    font.setPixelSize(FONT_SIZE);
    font.setFamily("deja vu sans mono");
    painter.setFont(font);

    width = std::max(std::max(sigma_width, lbound.width), rbound.width);
    if (width == 0)
        width = metrics.width(QChar('0'));

    height = sigma_height + rbound.height + lbound.height;
    center_height = sigma_height / 2 + rbound.height;
}

void Sigma::draw(int x, int y, QPainter &painter, bool cursor)
{
    // painter.setPen(Qt::red);
    // painter.drawRect(brect);
    //
    // painter.setPen(Qt::black);
    int y_mid = y + height - center_height;

    /* Lower bound */
    int x_lbound = x + (width - lbound.width) / 2;
    int y_lbound = y_mid + sigma_height / 2;
    lbound.draw(x_lbound, y_lbound, painter,
                cursor && (cursor_location == SIGMA_CURSOR_LBOUND));


    /* Upper bound */
    int x_rbound = x + (width - rbound.width) / 2;
    int y_rbound = 0;// y_mid - sigma_height/2 - rbound.height;

    rbound.draw(x_rbound, y_rbound, painter,
                cursor && (cursor_location == SIGMA_CURSOR_RBOUND));

    /* Sigma */
    QFont font = painter.font();
    font.setPixelSize(FONT_SIZE * 2);
    font.setFamily("dejavu math tex gyre");
    painter.setFont(font);

    QRect bsigma = QRect(x + (width - sigma_width) / 2,
                         y_mid - sigma_height / 2, sigma_width, sigma_height);
    painter.drawText(bsigma, Qt::AlignHCenter | Qt::AlignVCenter, QString("∑"));
    font.setPixelSize(FONT_SIZE);
    font.setFamily("dejavu sans Monospace");
    painter.setFont(font);
}


QPoint Sigma::getCursorCoordinates(void)
{
    if (cursor_location == SIGMA_CURSOR_RBOUND) {
        QPoint posInChild = rbound.getCursorCoordinates();
        size_t xPos = posInChild.x() + (width - rbound.width) / 2;
        size_t yPos = posInChild.y();
        return QPoint(xPos, yPos);
    } else {
        QPoint posInChild = lbound.getCursorCoordinates();
        size_t xPos = posInChild.x() + (width - lbound.width) / 2;
        size_t yPos = rbound.height + sigma_height + posInChild.y();
        return QPoint(xPos, yPos);
    }
    return QPoint(0, 0);
}