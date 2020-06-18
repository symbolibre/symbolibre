#include "Root.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>

#include <Qt>
#include <QFont>
#include <QRect>

Root::Root(void) : InternalEditionNode(1), rootWidth(-1)
{

}

QString Root::getNodeType() const
{
    return "sqrt";
}

void Root::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "SQUAREROOT\n";
    children[0].ascii(shift + 1, cc);
}

QString Root::getText(void) const
{
    QString str = "sqrt(";
    str.insert(str.size(), children[0].getText());
    str.push_back(')');
    return str;
}

bool Root::dropCursor(movedir dir)
{
    return children[0].dropCursor(dir);
}

bool Root::empty(void) const
{
    return children[0].empty();
}

void Root::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    children[0].computeDimensions(painter, 0, 0);

    /* Computing width */

    rootWidth     = ROOT_WIDTH;
    width         = children[0].width  + rootWidth + 2 * ROOT_VSPACE;
    height        = children[0].height + ROOT_HSPACE;
    center_height = children[0].center_height;
}

void Root::draw(int x, int y, QPainter &painter, bool cursor)
{
    /* Draw the top bar */
    painter.drawLine(x + rootWidth - 1, y, x + width - ROOT_VSPACE, y);

    /* Draw the 'square root' symbol : sets aliasing ON */
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(x + rootWidth - 1, y + 1,
                     x + rootWidth / 3, y + height);
    painter.drawLine(x, y + 2 * height / 3, x + rootWidth / 3, y + height);
    painter.setRenderHint(QPainter::Antialiasing, false);


    /* Then, draw the expression */
    children[0].draw(x + rootWidth + ROOT_VSPACE,
                     y + ROOT_HSPACE, painter, cursor);
}

QPoint Root::getCursorCoordinates(void)
{
    QPoint posInChild = children[0].getCursorCoordinates();
    size_t xPos = posInChild.x() + rootWidth + ROOT_VSPACE;
    size_t yPos = posInChild.y() + ROOT_HSPACE;
    return QPoint(xPos, yPos);
}
