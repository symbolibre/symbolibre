#include "Root.hpp"
#include "Flow.hpp"
#include <iostream>
#include <string>

#include <Qt>
#include <QFont>
#include <QRect>

Root::Root(void) : InternalEditionNode(), expression(), rootWidth(-1)
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
    expression.ascii(shift + 1, cc);
}

std::string Root::getText(void) const
{
    std::string str = "sqrt(";
    str.insert(str.size(), expression.getText());
    str.push_back(')');
    return str;
}

bool Root::dropCursor(movedir dir)
{
    return expression.dropCursor(dir);
}

bool Root::empty(void) const
{
    return expression.empty();
}

EditionNode *Root::getActiveChild(void)
{
    return &expression;
}

void Root::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    expression.computeDimensions(painter, 0, 0);

    /* Computing width */

    rootWidth     = ROOT_WIDTH;
    width         = expression.width  + rootWidth + 2 * ROOT_VSPACE;
    height        = expression.height + ROOT_HSPACE;
    center_height = expression.center_height;
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


    /* Then, draw the 'expression' */
    expression.draw(x + rootWidth + ROOT_VSPACE,
                    y + ROOT_HSPACE, painter, cursor);
}

QPoint Root::getCursorCoordinates(void)
{
    QPoint posInChild = expression.getCursorCoordinates();
    size_t xPos = posInChild.x() + rootWidth + ROOT_VSPACE;
    size_t yPos = posInChild.y() + ROOT_HSPACE;
    return QPoint(xPos, yPos);
}

std::vector<Flow *> Root::getChildren()
{
    return {&expression};
}
