#include "EditionArea.hpp"
#include <QString>
#include <algorithm>
#include <Qt>

/* ****************************************************************** */
/* **********************       TEXT NODE      ********************** */
/* ****************************************************************** */


EditionArea::EditionArea(std::string text, int cursor_pos) : EditionNode(),
    text(text), cursor_pos(cursor_pos)
{

}

std::string EditionArea::getText(void) const
{
    return text;
}

int EditionArea::getCursorPos(void) const
{
    return cursor_pos;
}

void EditionArea::set_to(std::string str)
{
    text = std::move(str);
}
void EditionArea::append(std::string str)
{
    text.insert(text.size(), str);
}

void EditionArea::cutAtCursor(std::string &cut)
{
    cut = text.substr(cursor_pos, text.size() - cursor_pos);
    text.erase(cursor_pos, text.size() - cursor_pos);
}

void EditionArea::ascii(int shift, bool cc)
{
    /* Text nodes have only one child so it's easy */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (cc) {
        std::cout << " └*\"";
        for (int i = 0; i < cursor_pos; i++)
            std::cout << text[i];
        std::cout << "▒" ;
        for (int i = cursor_pos; i < (int)text.size(); i++)
            std::cout << text[i];
        std::cout << "\"\n";
    } else
        std::cout << " └ \"" << text << "\"\n";
}

bool EditionArea::dropCursor(movedir dir)
{
    /* I could do a switch, but the syntax is ugly on my text editor. */
    if (dir == MRIGHT || dir == MDOWN)
        cursor_pos = text.size();
    else if (dir == MLEFT  || dir == MUP)
        cursor_pos = 0;

    return true;
}

bool EditionArea::empty(void) const
{
    return text.size() == 0;
}
void EditionArea::clear(void)
{
    cursor_pos = 0;
    text.clear();
}

bool EditionArea::reachedRight(void)
{
    return cursor_pos == (int) text.size();
}
bool EditionArea::reachedLeft(void)
{
    return cursor_pos == 0;
}

bool EditionArea::editMoveRight(void)
{
    if (reachedRight())
        return false;
    else
        cursor_pos ++;
    return true; /* And... this is not ambiguous :-) */
}

bool EditionArea::editMoveLeft(void)
{
    if (reachedLeft())
        return false;
    else
        cursor_pos --;
    return true;
}

bool EditionArea::editMoveUp(void)
{
    // FIXME why?
    cursor_pos = text.size();
    return false;
}

bool EditionArea::editMoveDown(void)
{
    cursor_pos = 0;
    return false;
}

bool EditionArea::editDelete(void)
{
    if (cursor_pos) {
        cursor_pos--;
        text.erase(cursor_pos, 1);
        return true;
    } else
        return false; /* nothing deleted */
}

void EditionArea::editString(const std::string &str)
{
    text.insert(cursor_pos, str);
    cursor_pos += str.size();
}

bool EditionArea::insert(EditionNode *newnode)
{
    if (auto area = dynamic_cast<EditionArea *>(newnode)) {
        editString(area->getText());
        cursor_pos += area->cursor_pos;
        delete newnode;
        return true;
    }

    return false;
}

void EditionArea::computeDimensions(QPainter &painter, int /**/, int /**/)
{
    QFontMetrics metrics = painter.fontMetrics();
    QRect br = metrics.boundingRect(QString::fromStdString(text));

    width  = br.width();
    height = std::max(FONT_SIZE, br.height());
    center_height = height / 2; // FIXME?
}

void EditionArea::draw(int x, int y, QPainter &painter, bool cursor)
{
    QRect brect = QRect(x, y, width, height);

    //painter.setPen(Qt::yellow);
    //painter.drawRect(brect);
    //painter.setPen(Qt::black);


    painter.drawText(brect, Qt::AlignHCenter | Qt::AlignBottom,
                     QString::fromStdString(text));

    /* About printing the cursor: */
    if (cursor) {
        /* We have to make measurements to find the location of the cursor */
        painter.setPen(Qt::red);
        QFontMetrics metrics = painter.fontMetrics();
        QFont font           = painter.font();

        int left_width = metrics.width(QString::fromStdString(text), cursor_pos);
        QRect crect = QRect(x + left_width - font.pixelSize() / 2,
                            y, font.pixelSize(), height);

        painter.drawText(crect, Qt::AlignHCenter | Qt::AlignVCenter, QString("│"));
        painter.setPen(Qt::black);
        //painter.drawRect(crect);
    }
}

QPoint EditionArea::getCursorCoordinates(void)
{
    /* Need the font to be monospace */
    size_t len  = text.length();
    size_t xPos = len ? width / len * cursor_pos : 0;
    size_t yPos = 0;
    return QPoint(xPos, yPos);
}
