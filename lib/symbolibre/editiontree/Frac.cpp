#include "Frac.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Frac::Frac(QString strnum) : InternalEditionNode(2)
{
    children[0] = Flow(strnum);
}

QString Frac::getNodeType() const
{
    return "frac";
}

void Frac::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "FRACTION\n";
    children[idx_num].ascii(shift + 1, cc && active_child_idx == idx_num);
    children[idx_den].ascii(shift + 1, cc && active_child_idx == idx_den);
}

QString Frac::getText(void) const
/* FIXME : awfull complexity */
{
    QString str = "(";
    str.insert(str.size(), children[idx_num].getText());
    str.insert(str.size(), ")/(");
    str.insert(str.size(), children[idx_den].getText());
    str.insert(str.size(), ")");
    return str;
}

bool Frac::dropCursor(movedir dir)
{
    /* Convention: the cursor is ALWAYS dropped on top of the fraction */
    active_child_idx = idx_num;
    return children[idx_num].dropCursor(dir);
}

bool Frac::empty(void) const
{
    return false;
    //return children[idx_num].empty() && children[idx_den].empty();
}

bool Frac::editMoveUp(void)
{
    if (active_child_idx == idx_num)
        return false;

    active_child_idx = idx_num;
    children[idx_num].dropCursor(MUP);
    return true;
}

bool Frac::editMoveDown(void)
{
    if (active_child_idx == idx_den)
        return false;

    active_child_idx = idx_den;
    children[idx_den].dropCursor(MDOWN);
    return true;
}

void Frac::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    children[idx_num].computeDimensions(painter, 0, 0);
    children[idx_den].computeDimensions(painter, 0, 0);

    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString("0"));
    int min_height = br.height();

    width  = std::max(children[idx_num].width, children[idx_den].width);
    if (width == 0)
        width = metrics.horizontalAdvance(QChar('0'));

    width += FRAC_SPACE;
    height = FRAC_UP_SPACE + FRAC_DOWN_SPACE +
             std::max(min_height, children[idx_num].height) +
             std::max(min_height, children[idx_den].height);
    center_height = FRAC_DOWN_SPACE +
                    std::max(min_height, children[idx_den].height);
}

void Frac::draw(int x, int y, QPainter &painter, bool cursor)
{
    /*QRect brect = QRect(x, y, width, height);

    painter.setPen(Qt::red);
    painter.drawRect(brect);
    painter.setPen(Qt::black);*/

    /* children[idx_num] */
    int x_numerator = x + (width - children[idx_num].width) / 2;
    children[idx_num].draw(x_numerator, y, painter, cursor && active_child_idx == idx_num);

    /* frac line */
    int y_line = y + height - center_height;
    painter.drawLine(x + FRAC_SPACE / 2, y_line, x - FRAC_SPACE / 2 + width - 1, y_line);

    /* Denominator */
    int x_denominator = x + (width - children[idx_den].width) / 2;
    int y_denominator = y + height - center_height + FRAC_UP_SPACE;

    children[idx_den].draw(x_denominator, y_denominator, painter,
                           cursor && active_child_idx == idx_den);
}

QPoint Frac::getCursorCoordinates(void)
{
    if (active_child_idx == idx_num) {
        QPoint posInChild = children[idx_num].getCursorCoordinates();
        size_t xPos = posInChild.x() + (width - children[idx_num].width) / 2;
        size_t yPos = posInChild.y();
        return QPoint(xPos, yPos);
    } else {
        QPoint posInChild = children[idx_den].getCursorCoordinates();
        size_t xPos = posInChild.x() + (width - children[idx_den].width) / 2;
        size_t yPos = posInChild.y() + height - center_height + FRAC_UP_SPACE;
        return QPoint(xPos, yPos);
    }
}
