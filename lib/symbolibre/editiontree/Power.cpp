#include "Power.hpp"
#include "Flow.hpp"
#include <iostream>


#include <Qt>
#include <QFont>
#include <QRect>

Power::Power(void) : InternalEditionNode(1)
{

}

QString Power::getNodeType() const
{
    return "pow";
}

void Power::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "POWER\n";
    children[0].ascii(shift + 1, cc);
}

QString Power::getText(void) const
{
    QString str = "^(";
    str.insert(str.size(), children[0].getText());
    str.push_back(')');
    return str;
}

bool Power::dropCursor(movedir dir)
{
    return children[0].dropCursor(dir);
}

bool Power::empty(void) const
{
    return children[0].empty();
}

void Power::computeDimensions(QPainter &painter, int prev_height, int prev_cheight)
{
    children[0].computeDimensions(painter, 0, 0);

    /* Right now, there is no adjustment */
    width         = children[0].width  ;
    height        = children[0].height
                    + std::max(0, prev_height - children[0].center_height - prev_cheight);
    center_height = 0;
    // Here is the trick - FIXME because it is not robust.
}

void Power::draw(int x, int y, QPainter &painter, bool cursor)
{
    /* Draw the expression */
    children[0].draw(x, y, painter, cursor);
}

QPoint Power::getCursorCoordinates(void)
{
    return children[0].getCursorCoordinates();
}
