#include "Operator.hpp"
#include <iostream>

Operator::Operator(char ac, QString qs) : EditionNode(),
    qstring(qs), achar(ac)
{

}

void Operator::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "OPERATOR " << achar << '\n';
}

std::string Operator::getText(void) /* DO NOT USE IT */
{
    std::string op = "."; // help T^T
    op[0] = achar;
    return op;
}

void Operator::append(std::string &)
{

}

bool Operator::dropCursor(movedir)
{
    return false;
}

bool Operator::empty(void)
{
    return false;
}

bool Operator::editClear(void)
{
    return false;
}

void Operator::computeDimensions(QPainter &painter)
{
    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(qstring);

    width  = br.width();
    height = std::max(FONT_SIZE, br.height());
    center_height = height / 2; // FIXME ?
}

void Operator::draw(int x, int y, QPainter &painter, bool)
{
    QRect brect = QRect(x, y, width, height);

    painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, qstring);
}
