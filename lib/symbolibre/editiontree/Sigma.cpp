// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Sigma.hpp"
#include "Flow.hpp"
#include "FontResizer.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Sigma::Sigma(void) : InternalEditionNode(3),
    sigma_height(0), sigma_width(0), m_lpar(LPAREN), m_rpar(RPAREN)
{
}

QString Sigma::getNodeType() const
{
    return "sum";
}

void Sigma::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << "SIGMA\n";

    for (int i = 0; i < shift + 1; i++)
        std::cout << "  ";
    std::cout << " └" << (cc && active_child_idx == idx_lbound ? '*' : ' ')
              << "LBOUND\n";
    children[idx_lbound].ascii(shift + 2, cc && (active_child_idx == idx_lbound));

    for (int i = 0; i < shift + 1; i++)
        std::cout << "  ";
    std::cout << " └" << (cc && active_child_idx == idx_rbound ? '*' : ' ')
              << "RBOUND\n";
    children[idx_rbound].ascii(shift + 2, cc && (active_child_idx == idx_rbound));

    for (int i = 0; i < shift + 1; i++)
        std::cout << "  ";
    std::cout << " └" << (cc && active_child_idx == idx_term ? '*' : ' ')
              << "TERM\n";
    children[idx_term].ascii(shift + 2, cc && (active_child_idx == idx_term));

}

QString Sigma::getText(void) const
{
    // TODO: Properly parse lower bound as <var> := <lvalue>
    QString lower_bound = children[idx_lbound].getText();
    QString var;

    if (lower_bound.mid(1,2) == ":=") {
        var = lower_bound.mid(0,1);
        lower_bound = lower_bound.mid(3);
    }

    return QString("sum(") + children[idx_term].getText() + "," + var + "," +
        lower_bound + "," + children[idx_rbound].getText() + ")";
}

bool Sigma::dropCursor(movedir dir)
{
    if (dir == MLEFT || dir == MDOWN) {
        active_child_idx = idx_lbound;
        return children[idx_lbound].dropCursor(MLEFT);
    } else {
        active_child_idx = idx_term;
        return children[idx_term].dropCursor(MRIGHT);
    }
}

bool Sigma::empty(void) const
{
    return false;
}

bool Sigma::editMoveUp(void)
{
    if (active_child_idx != idx_lbound)
        return false;

    active_child_idx = idx_rbound;
    children[idx_rbound].dropCursor(MLEFT);
    return true;
}

bool Sigma::editMoveDown(void)
{
    if (active_child_idx != idx_rbound)
        return false;

    active_child_idx = idx_lbound;
    children[idx_lbound].dropCursor(MLEFT);
    return true;
}

bool Sigma::editMoveLeft(void)
{
    if (active_child_idx == idx_rbound)
    {
        active_child_idx = idx_lbound;
        children[idx_lbound].dropCursor(MRIGHT);
        return true;
    }

    if (active_child_idx == idx_term)
    {
        active_child_idx = idx_rbound;
        children[idx_rbound].dropCursor(MRIGHT);
        return true;
    }

    return false;
}

bool Sigma::editMoveRight(void)
{
    if (active_child_idx == idx_rbound)
    {
        active_child_idx = idx_term;
        children[idx_term].dropCursor(MLEFT);
        return true;
    }

    if (active_child_idx == idx_lbound)
    {
        active_child_idx = idx_rbound;
        children[idx_rbound].dropCursor(MLEFT);
        return true;
    }

    return false;
}

void Sigma::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
{
    const auto pointSize = painter.fontInfo().pointSize();

    {
        const auto f = FontResizer(painter, pointSize * 2 / 3);
        children[idx_lbound].computeDimensions(painter, 0, 0);
        children[idx_rbound].computeDimensions(painter, 0, 0);
    }

    children[idx_term].computeDimensions(painter, 0, 0);

    {
        const auto f = FontResizer(painter, pointSize * 3 / 2);

        // FIXME: Size of bounding rect is a very bad approximation of the size
        // of the character here; use another method or an image. For now the
        // size is tied to the current font options.
        QFontMetricsF metrics(painter.font());
        // QRectF br = metrics.boundingRect(QString("∑"));
        sigma_height = 15;
        sigma_width = 10;
    }

    // Width of the sigma symbol with bounds, and full width
    m_group_width = std::max({sigma_width, children[idx_lbound].width, children[idx_rbound].width});
    width = m_group_width + m_lpar.width + children[idx_term].width + m_rpar.width;

    qreal ascent1  = children[idx_rbound].height + sigma_height - SIGMA_DESCENT;
    qreal descent1 = children[idx_lbound].height + SIGMA_DESCENT;
    qreal ascent2  = children[idx_term].ascent;
    qreal descent2 = children[idx_term].height - ascent2;

    height = std::max({ ascent1, ascent2 }) + std::max({ descent1, descent2 });

    // Update the height of the parentheses to be that of this node
    m_lpar.computeDimensions(painter, 0, 0);
    m_rpar.computeDimensions(painter, 0, 0);
    qreal par_height = std::max({ sigma_height, children[idx_term].height });
    m_lpar.height = par_height;
    m_rpar.height = par_height;

    ascent = std::max({ ascent1, ascent2 });
}

void Sigma::draw(qreal x, qreal y, QPainter &painter, bool cursor)
{
    InternalEditionNode::draw(x, y, painter, cursor);

    const auto pointSize = painter.fontInfo().pointSize();

    {
        FontResizer f(painter, pointSize * 2 / 3);

        /* Lower bound */
        qreal x_lbound = x + (m_group_width - children[idx_lbound].width) / 2;
        qreal y_lbound = y + ascent + SIGMA_DESCENT;
        children[idx_lbound].draw(x_lbound, y_lbound, painter,
                                  cursor && (active_child_idx == idx_lbound));

        /* Upper bound */
        qreal x_rbound = x + (m_group_width - children[idx_rbound].width) / 2;
        qreal y_rbound = y;
        children[idx_rbound].draw(x_rbound, y_rbound, painter,
                                  cursor && (active_child_idx == idx_rbound));
    }

    /* Sigma */
    {
        FontResizer f(painter, pointSize * 3 / 2);

        // FIXME: Height of sigma symbol is poorly estimated by the bounding
        // box, so an offset of 4 is applied here to compensate; see also in
        // computeDimensions().
        QRectF bsigma = QRectF(x + (m_group_width - sigma_width) / 2,
                             y + children[idx_rbound].height - 4, sigma_width, sigma_height + 4);
        painter.drawText(bsigma, Qt::AlignHCenter | Qt::AlignTop, QString("∑"));
    }

    // Parentheses
    qreal term_y = y + ascent - children[idx_term].ascent;

    qreal par_y = (children[idx_term].height < sigma_height) ?
        y + children[idx_rbound].height + (children[idx_term].height - children[idx_term].ascent) - SIGMA_DESCENT :
        term_y;

    m_lpar.draw(x + m_group_width, par_y, painter, false);
    m_rpar.draw(x + width - m_rpar.width, par_y, painter, false);

    // FIXME: Term position
    // Try 1+sum(a:=0,n,1+n/2) and see that the "+" signs are lined up (good)
    // but the "1" digits aren't (bad). The "1+" isn't consistent.
    children[idx_term].draw(x + m_group_width + m_lpar.width, term_y, painter,
        cursor && (active_child_idx == idx_term));
}
