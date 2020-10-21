// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

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

void Variable::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
{
    QFontMetricsF metrics(painter.font());
    QRectF br = metrics.boundingRect(name);

    width  = br.width();
    height = std::max(metrics.height(), br.height());
    ascent = metrics.ascent();
}

void Variable::draw(qreal x, qreal y, QPainter &painter, bool)
{
    EditionNode::draw(x, y, painter, false);
    QRectF brect = QRectF(x, y, width, height);
    painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom, name);
}
