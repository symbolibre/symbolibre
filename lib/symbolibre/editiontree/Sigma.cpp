#include "Sigma.hpp"
#include "Flow.hpp"
#include "FontResizer.hpp"
#include <iostream>
#include <string>
#include <algorithm>

Sigma::Sigma(void) : InternalEditionNode(2),
    sigma_height(0), sigma_width(0)
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
}

QString Sigma::getText(void) const
{
    QString str = "sum(";
    str.insert(str.size(), children[idx_lbound].getText());
    str.insert(str.size(), ",");
    str.insert(str.size(), children[idx_rbound].getText());
    str.insert(str.size(), ")");
    return str;
}

bool Sigma::dropCursor(movedir dir)
{
    if (dir == MLEFT || dir == MDOWN) {
        active_child_idx = idx_rbound;
        return children[idx_rbound].dropCursor(MLEFT);
    } else {
        active_child_idx = idx_lbound;
        return children[idx_lbound].dropCursor(MRIGHT);
    }
}

bool Sigma::empty(void) const
{
    return false;
}

bool Sigma::editMoveUp(void)
{
    if (active_child_idx == idx_rbound)
        return false;

    active_child_idx = idx_rbound;
    children[idx_rbound].dropCursor(MLEFT);
    return true;
}

bool Sigma::editMoveDown(void)
{
    if (active_child_idx == idx_lbound)
        return false;

    active_child_idx = idx_lbound;
    children[idx_lbound].dropCursor(MLEFT);
    return true;
}

void Sigma::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    const auto pointSize = painter.fontInfo().pointSize();

    {
        const auto f = FontResizer(painter, pointSize * 2 / 3);
        children[idx_lbound].computeDimensions(painter, 0, 0);
        children[idx_rbound].computeDimensions(painter, 0, 0);
    }

    {
        const auto f = FontResizer(painter, pointSize * 3 / 2);

        QFontMetrics metrics = painter.fontMetrics();
        QRect br = metrics.boundingRect(QString("∑"));
        sigma_height = br.height();
        sigma_width = br.width();
    }

    width = std::max({sigma_width, children[idx_lbound].width, children[idx_rbound].width});

    height = sigma_height + children[idx_rbound].height + children[idx_lbound].height;
    ascent = sigma_height + children[idx_rbound].height;
}

void Sigma::draw(int x, int y, QPainter &painter, bool cursor)
{
    InternalEditionNode::draw(x, y, painter, cursor);

    const auto pointSize = painter.fontInfo().pointSize();

    {
        FontResizer f(painter, pointSize * 2 / 3);

        /* Lower bound */
        int x_lbound = x + (width - children[idx_lbound].width) / 2;
        int y_lbound = y + ascent;
        children[idx_lbound].draw(x_lbound, y_lbound, painter,
                                  cursor && (active_child_idx == idx_lbound));

        /* Upper bound */
        int x_rbound = x + (width - children[idx_rbound].width) / 2;
        int y_rbound = y;
        children[idx_rbound].draw(x_rbound, y_rbound, painter,
                                  cursor && (active_child_idx == idx_rbound));
    }

    /* Sigma */
    {
        FontResizer f(painter, pointSize * 3 / 2);

        // FIXME why is +1 needed here?
        QRect bsigma = QRect(x + (width - sigma_width) / 2,
                             y + children[idx_rbound].height, sigma_width, sigma_height + 1);
        painter.drawText(bsigma, Qt::AlignHCenter | Qt::AlignVCenter, QString("∑"));
    }
}
