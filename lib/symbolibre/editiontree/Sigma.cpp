#include "Sigma.hpp"
#include "Flow.hpp"
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
    children[idx_lbound].computeDimensions(painter, 0, 0);
    children[idx_rbound].computeDimensions(painter, 0, 0);

    QFont font = painter.font(), largeFont = painter.font();
    largeFont.setPointSize(painter.fontInfo().pointSize() * 2);
    painter.setFont(largeFont);

    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString("∑"));
    sigma_height = br.height();
    sigma_width = br.width();

    painter.setFont(font);

    width = std::max({sigma_width, children[idx_lbound].width, children[idx_rbound].width});
    if (width == 0)
        width = metrics.horizontalAdvance(QChar('0'));

    height = sigma_height + children[idx_rbound].height + children[idx_lbound].height;
    center_height = sigma_height / 2 + children[idx_rbound].height;
}

void Sigma::draw(int x, int y, QPainter &painter, bool cursor)
{
    /* Lower bound */
    int x_lbound = x + (width - children[idx_lbound].width) / 2;
    int y_lbound = y + center_height + sigma_height / 2;
    children[idx_lbound].draw(x_lbound, y_lbound, painter,
                              cursor && (active_child_idx == idx_lbound));


    /* Upper bound */
    int x_rbound = x + (width - children[idx_rbound].width) / 2;
    int y_rbound = y;

    children[idx_rbound].draw(x_rbound, y_rbound, painter,
                              cursor && (active_child_idx == idx_rbound));

    /* Sigma */
    QFont font = painter.font(), largeFont = painter.font();
    largeFont.setPointSize(painter.fontInfo().pointSize() * 2);
    painter.setFont(largeFont);

    QRect bsigma = QRect(x + (width - sigma_width) / 2,
                         y + children[idx_rbound].height, sigma_width, sigma_height);
    painter.drawText(bsigma, Qt::AlignHCenter | Qt::AlignVCenter, QString("∑"));

    painter.setFont(font);
}


QPoint Sigma::getCursorCoordinates(void)
{
    if (active_child_idx == idx_rbound) {
        QPoint posInChild = children[idx_rbound].getCursorCoordinates();
        size_t xPos = posInChild.x() + (width - children[idx_rbound].width) / 2;
        size_t yPos = posInChild.y();
        return QPoint(xPos, yPos);
    } else {
        QPoint posInChild = children[idx_lbound].getCursorCoordinates();
        size_t xPos = posInChild.x() + (width - children[idx_lbound].width) / 2;
        size_t yPos = children[idx_rbound].height + sigma_height + posInChild.y();
        return QPoint(xPos, yPos);
    }
    return QPoint(0, 0);
}
