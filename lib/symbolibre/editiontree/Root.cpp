// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

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

void Root::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
{
    children[0].computeDimensions(painter, 0, 0);

    /* Computing width */

    rootWidth     = painter.fontInfo().pixelSize() * 2 / 3;
    width         = children[0].width  + rootWidth + 2 * ROOT_VSPACE;
    height        = children[0].height + ROOT_HSPACE;
    ascent = children[0].ascent + ROOT_HSPACE;
}

void Root::draw(qreal x, qreal y, QPainter &painter, bool cursor)
{
    InternalEditionNode::draw(x, y, painter, cursor);

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
