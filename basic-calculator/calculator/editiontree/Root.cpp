#include "Root.hpp"
#include "Flow.hpp"
#include <memory>
#include <iostream>
#include <string>

#include <Qt>
#include <QFont>
#include <QRect>

Root::Root(void) /* I'm Root */
{
    ntype         = SQUAREROOT;
    width         =    0;
    height        =    0;
    center_height =    0;
    cursor_pos    =    0;

    expression = std::make_unique<Flow>(FLOW);
    rootWidth  = -1;
}

void Root::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "SQUAREROOT\n";
    expression->ascii(shift + 1, cc);
    return;
}

std::string Root::getText(void)
{
    std::string str = "sqrt(";
    str.insert(str.size(), expression->getText());
    str.push_back(')');
    return str;
}

void Root::append(std::string &)
{
    return;
}

bool Root::dropCursor(movedir dir)
{
    return expression->dropCursor(dir);
}

void Root::cutAtCursor(std::string &)
{
    return;
}

bool Root::empty(void)
{
    return expression->empty();
}

bool Root::reachedRight(void)
{
    return expression->reachedRight();
}

bool Root::reachedLeft(void)
{
    return expression->reachedLeft();
}

bool Root::editMoveRight(void)
{
    return expression->editMoveRight();
}

bool Root::editMoveLeft(void)
{
    return expression->editMoveLeft();
}

bool Root::editMoveUp(void)
{
    return expression->editMoveUp();
}

bool Root::editMoveDown(void)
{
    return expression->editMoveDown();
}

bool Root::editDelete(void)
{
    expression->editDelete();
    return true; /* FIXME ? maybe should return child value */
    /* see edition experience */
}

bool Root::editClear(void)
{
    expression->editClear();
    return true;
}

bool Root::editChar(char symbol)
{
    return expression->editChar(symbol);
}

bool Root::editParen(nodetype paren_type)
{
    return expression->editParen(paren_type);
}

bool Root::editFrac(void)
{
    return expression->editFrac();
}

bool Root::editRoot(void)
{
    return expression->editRoot();
}

void Root::computeDimensions(QPainter &painter)
{
    expression->computeDimensions(painter);

    /* Computing width */

    rootWidth     = ROOT_WIDTH;
    width         = expression->width  + rootWidth + ROOT_VSPACE;
    height        = expression->height + ROOT_HSPACE;
    center_height = expression->center_height;

    return;
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
    expression->draw(x + rootWidth, y + ROOT_HSPACE, painter, cursor);

    return;
}