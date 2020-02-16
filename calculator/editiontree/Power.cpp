#include "Power.hpp"
#include "Flow.hpp"
#include <iostream>


#include <Qt>
#include <QFont>
#include <QRect>

Power::Power(void) : InternalEditionNode(), expression()
{

}

void Power::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "POWER\n";
    expression.ascii(shift + 1, cc);
}

std::string Power::getText(void) const
{
    std::string str = "^(";
    str.insert(str.size(), expression.getText());
    str.push_back(')');
    return str;
}

bool Power::dropCursor(movedir dir)
{
    return expression.dropCursor(dir);
}

bool Power::empty(void) const
{
    return expression.empty();
}

EditionNode *Power::getActiveChild(void)
{
    return &expression;
}

void Power::computeDimensions(QPainter &painter, int prev_height, int prev_cheight)
{
    expression.computeDimensions(painter, 0, 0);

    /* Right now, there is no adjustment */
    width         = expression.width  ;
    height        = expression.height
                    + std::max(0, prev_height - expression.center_height - prev_cheight);
    center_height = 0;
    // Here is the trick - FIXME because it is not robust.
}

void Power::draw(int x, int y, QPainter &painter, bool cursor)
{
    /* Draw the 'expression' */
    expression.draw(x, y, painter, cursor);
}

QPoint Power::getCursorCoordinates(void)
{
    return expression.getCursorCoordinates();
}
