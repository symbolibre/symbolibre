#include "Variable.hpp"
#include <iostream>

Variable::Variable(QString name) : EditionNode(), name(name)
{

}

void Variable::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "VARIABLE " << name.toStdString() << '\n';
}

QString Variable::getText(void) const
{
    return "(" + name + ")";
}

bool Variable::dropCursor(movedir)
{
    return false;
}

bool Variable::empty(void) const
{
    return false;
}

void Variable::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(name);

    width  = br.width();
    height = std::max(metrics.height(), br.height());
    ascent = metrics.ascent();
}

void Variable::draw(int x, int y, QPainter &painter, bool)
{
    EditionNode::draw(x, y, painter, false);
    QRect brect = QRect(x, y, width, height);
    painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, name);
}
