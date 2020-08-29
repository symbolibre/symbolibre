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

QString Operator::getText(void) const
{
    return QString(1, achar);
}

bool Operator::dropCursor(movedir)
{
    return false;
}

bool Operator::empty(void) const
{
    return false;
}

void Operator::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
{
    QFontMetricsF metrics(painter.font());
    QRectF br = metrics.boundingRect(qstring);

    width  = br.width();
    height = std::max(metrics.height(), br.height());
    ascent = metrics.ascent();
}

void Operator::draw(qreal x, qreal y, QPainter &painter, bool)
{
    EditionNode::draw(x, y, painter, false);

    QRectF brect = QRectF(x, y, width, height);
    painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, qstring);
}
