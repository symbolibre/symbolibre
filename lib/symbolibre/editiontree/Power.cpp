#include "Power.hpp"
#include "Flow.hpp"
#include "FontResizer.hpp"
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

void Power::computeDimensions(QPainter &painter, int prev_height, int prev_ascent)
{
    {
        FontResizer f(painter, painter.fontInfo().pointSize() * 2 / 3);
        children[0].computeDimensions(painter, 0, 0);
    }
    width         = children[0].width;

    const auto &metrics = painter.fontMetrics();
    if (prev_height) {
        height = children[0].height + prev_height - metrics.ascent() / 2;
        ascent = children[0].height + prev_ascent - metrics.ascent() / 2;
    } else {
        height = children[0].height + metrics.height() - metrics.ascent() / 2;
        ascent = children[0].height + metrics.ascent() / 2;
    }
}

void Power::draw(int x, int y, QPainter &painter, bool cursor)
{
    FontResizer f(painter, painter.fontInfo().pointSize() * 2 / 3);
    children[0].draw(x, y, painter, cursor);
}

QPoint Power::getCursorCoordinates(void)
{
    return children[0].getCursorCoordinates();
}
