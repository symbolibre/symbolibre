#include "Integral.hpp"
#include "Flow.hpp"
#include "FontResizer.hpp"

Integral::Integral(bool definite): InternalEditionNode(definite ? 3 : 2),
    int_height(0), int_width(0), m_lpar(LPAREN), m_rpar(RPAREN),
    m_definite(definite)
{
}

QString Integral::getNodeType() const
{
    return m_definite ? "defint" : "int";
}

void Integral::ascii(int shift, bool cc)
{
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    std::cout << " └" << (cc ? '*' : ' ') << (m_definite?"DEFINT\n":"INT\n");

    if (m_definite)
    {
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

    for (int i = 0; i < shift + 1; i++)
        std::cout << "  ";
    std::cout << " └" << (cc && active_child_idx == idx_term ? '*' : ' ')
              << "TERM\n";
    children[idx_term].ascii(shift + 2, cc && (active_child_idx == idx_term));

    if (!m_definite)
    {
        for (int i = 0; i < shift + 1; i++)
            std::cout << "  ";
        std::cout << " └" << (cc && active_child_idx == idx_diff ? '*' : ' ')
                  << "DIFF";
        children[idx_diff].ascii(shift + 2, cc && (active_child_idx == idx_diff));
    }
}

QString Integral::getText(void) const
{
    if (m_definite)
    {
        // TODO: Properly parse lower bound as <var> := <lvalue>
        QString lower_bound = children[idx_lbound].getText();
        QString var;

        if (lower_bound.mid(1,2) == ":=") {
            var = lower_bound.mid(0,1);
            lower_bound = lower_bound.mid(3);
        }

        return QString("int(") + children[idx_term].getText() + "," + var + "," +
            lower_bound + "," + children[idx_rbound].getText() + ")";
    }
    else
    {
        return QString("int(") + children[idx_term].getText() + ")";
    }
}

bool Integral::dropCursor(movedir dir)
{
    if (dir == MLEFT || dir == MDOWN) {
        active_child_idx = m_definite ? idx_lbound : idx_term;
        return children[active_child_idx].dropCursor(MLEFT);
    } else {
        active_child_idx = m_definite ? idx_term : idx_diff;
        return children[active_child_idx].dropCursor(MRIGHT);
    }
}

bool Integral::empty(void) const
{
    return false;
}

bool Integral::editMoveUp(void)
{
    if (!m_definite || active_child_idx != idx_lbound)
        return false;

    active_child_idx = idx_rbound;
    children[idx_rbound].dropCursor(MLEFT);
    return true;
}

bool Integral::editMoveDown(void)
{
    if (!m_definite || active_child_idx != idx_rbound)
        return false;

    active_child_idx = idx_lbound;
    children[idx_lbound].dropCursor(MLEFT);
    return true;
}

bool Integral::editMoveLeft(void)
{
    if (m_definite && active_child_idx == idx_rbound)
    {
        active_child_idx = idx_lbound;
        children[idx_lbound].dropCursor(MRIGHT);
        return true;
    }

    if (m_definite && active_child_idx == idx_term)
    {
        active_child_idx = idx_rbound;
        children[idx_rbound].dropCursor(MRIGHT);
        return true;
    }

    if (!m_definite && active_child_idx == idx_diff)
    {
        active_child_idx = idx_term;
        children[idx_term].dropCursor(MRIGHT);
        return true;
    }

    return false;
}

bool Integral::editMoveRight(void)
{
    if (m_definite && active_child_idx == idx_rbound)
    {
        active_child_idx = idx_term;
        children[idx_term].dropCursor(MLEFT);
        return true;
    }

    if (m_definite && active_child_idx == idx_lbound)
    {
        active_child_idx = idx_rbound;
        children[idx_rbound].dropCursor(MLEFT);
        return true;
    }

    /* TODO: Add support for the differential
    if (!m_definite && active_child_idx == idx_term)
    {
        active_child_idx = idx_diff;
        children[idx_diff].dropCursor(MLEFT);
        return true;
    } */

    return false;
}

void Integral::computeDimensions(QPainter &painter, qreal /**/, qreal /**/)
{
    const auto pointSize = painter.fontInfo().pointSize();

    if (m_definite) {
        const auto f = FontResizer(painter, pointSize * 2 / 3);
        children[idx_lbound].computeDimensions(painter, 0, 0);
        children[idx_rbound].computeDimensions(painter, 0, 0);
    }

    children[idx_term].computeDimensions(painter, 0, 0);

    if (!m_definite)
        children[idx_diff].computeDimensions(painter, 0, 0);

    {
        const auto f = FontResizer(painter, pointSize * 3 / 2);

        // FIXME: Can't estimate size precisely; see Sigma
        QFontMetricsF metrics(painter.font());
        QRectF br = metrics.boundingRect(QString("∫"));
        int_height = 16;
        int_width = 6;
    }

    m_group_width = int_width;
    if (m_definite)
        m_group_width += std::max({ children[idx_lbound].width, children[idx_rbound].width });

    width = m_group_width + m_lpar.width + children[idx_term].width + m_rpar.width;
    /* TODO: Add support for the differential
    if (!m_definite)
        width += children[idx_diff].width; */

    m_group_height = int_height;
    if (m_definite)
        m_group_height += children[idx_lbound].height + children[idx_rbound].height - 2 * INT_OVERLAP;

    qreal ascent1 = m_definite ? int_height + children[idx_rbound].height - INT_OVERLAP : int_height;
    qreal ascent2 = children[idx_term].ascent;
    qreal ascent3 = m_definite ? 0 : children[idx_diff].ascent;

    qreal descent1 = m_definite ? children[idx_lbound].height : 0;
    qreal descent2 = children[idx_term].height - ascent2;
    qreal descent3 = m_definite ? 0 : children[idx_diff].height - ascent3;

    height = std::max({ ascent1, ascent2, ascent3 }) + std::max({ descent1, descent2, descent3 });

    // Update the height of the parentheses to be that of this node
    m_lpar.computeDimensions(painter, 0, 0);
    m_rpar.computeDimensions(painter, 0, 0);
    qreal par_height = std::max({ int_height, children[idx_term].height });
    m_lpar.height = par_height;
    m_rpar.height = par_height;

    ascent = std::max({ ascent1, ascent2, ascent3 });
}

void Integral::draw(qreal x, qreal y, QPainter &painter, bool cursor)
{
    InternalEditionNode::draw(x, y, painter, cursor);

    const auto pointSize = painter.fontInfo().pointSize();

    /* Integral symbol */
    {
        FontResizer f(painter, pointSize * 3 / 2);

        // FIXME: The symbol has 4 blank pixels on top of it when rendered.
        // Compensate by drawing it 4 pixels higher.
        QRectF bint = QRectF(x, y + ascent - int_height - 4, int_width,
            int_height + 4);
        painter.drawText(bint, Qt::AlignLeft | Qt::AlignTop, QString("∫"));
    }

    if (m_definite) {
        FontResizer f(painter, pointSize * 2 / 3);

        /* Lower bound */
        qreal y_lbound = y + ascent - INT_OVERLAP;
        children[idx_lbound].draw(x + int_width, y_lbound, painter,
                                  cursor && (active_child_idx == idx_lbound));
        /* Upper bound */
        qreal y_rbound = y + ascent - int_height + INT_OVERLAP - children[idx_rbound].height;
        children[idx_rbound].draw(x + int_width, y_rbound, painter,
                                  cursor && (active_child_idx == idx_rbound));
    }

    // Parentheses
    qreal term_y = y + ascent - children[idx_term].ascent;

    qreal par_y = (children[idx_term].height < int_height) ? y + ascent - int_height: term_y;

    m_lpar.draw(x + m_group_width, par_y, painter, false);
    m_rpar.draw(x + m_group_width + m_lpar.width + children[idx_term].width, par_y, painter, false);

    children[idx_term].draw(x + m_group_width + m_lpar.width, term_y, painter,
        cursor && (active_child_idx == idx_term));

    /* TODO: Add support for the differential
    if (!m_definite)
        children[idx_diff].draw(x + width - children[idx_diff].width,
            y + ascent - children[idx_diff].ascent, painter,
            cursor && (active_child_idx == idx_diff)); */
}
