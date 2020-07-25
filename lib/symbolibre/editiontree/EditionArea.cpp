#include "EditionArea.hpp"
#include <QString>
#include <QTextBoundaryFinder>
#include <QTextLayout>
#include <algorithm>

/* ****************************************************************** */
/* **********************       TEXT NODE      ********************** */
/* ****************************************************************** */


EditionArea::EditionArea(QString text, int cursor_pos) : EditionNode(),
    text(text),
    cursor_pos(cursor_pos),
    mCursorCoords()
{

}

QString EditionArea::getText(void) const
{
    return text;
}

int EditionArea::getCursorPos(void) const
{
    return cursor_pos;
}

void EditionArea::setCursorPos(int pos)
{
    cursor_pos = pos;
}

void EditionArea::setText(QString str)
{
    text = str;
}

void EditionArea::append(QString str)
{
    text.insert(text.size(), str);
}

void EditionArea::cutAtCursor(QString &cut)
{
    cut = text.right(text.size() - cursor_pos);
    text.truncate(cursor_pos);
}

void EditionArea::ascii(int shift, bool cc)
{
    /* Text nodes have only one child so it's easy */
    for (int i = 0; i < shift; i++)
        std::cout << "  ";
    if (cc) {
        std::cout << " └*\"";
        std::cout << text.left(cursor_pos).toStdString();
        std::cout << "▒" ;
        std::cout << text.right(text.size() - cursor_pos).toStdString();
        std::cout << "\"\n";
    } else
        std::cout << " └ \"" << text.toStdString() << "\"\n";
}

bool EditionArea::dropCursor(movedir dir)
{
    if (dir == MRIGHT || dir == MDOWN)
        cursor_pos = text.size();
    else if (dir == MLEFT  || dir == MUP)
        cursor_pos = 0;

    return true;
}

bool EditionArea::empty(void) const
{
    return text.isEmpty();
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

    auto cursor = QTextBoundaryFinder(QTextBoundaryFinder::Grapheme, text);
    cursor.setPosition(cursor_pos);
    cursor_pos = cursor.toNextBoundary();
    return true;
}

bool EditionArea::editMoveLeft(void)
{
    if (reachedLeft())
        return false;

    auto cursor = QTextBoundaryFinder(QTextBoundaryFinder::Grapheme, text);
    cursor.setPosition(cursor_pos);
    cursor_pos = cursor.toPreviousBoundary();
    return true;
}

bool EditionArea::editMoveUp(void)
{
    cursor_pos = 0;
    return false;
}

bool EditionArea::editMoveDown(void)
{
    cursor_pos = text.size();
    return false;
}

bool EditionArea::editDelete(void)
{
    if (!cursor_pos)
        return false;

    auto cursor = QTextBoundaryFinder(QTextBoundaryFinder::Grapheme, text);
    cursor.setPosition(cursor_pos);
    const int start_pos = cursor.toPreviousBoundary();
    text.remove(start_pos, cursor_pos - start_pos);
    cursor_pos = start_pos;
    return true;
}

void EditionArea::editString(const QString &str)
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
    QTextLayout layout(text, painter.font());
    layout.beginLayout();
    auto line = layout.createLine();
    layout.endLayout();

    width  = line.naturalTextWidth() + 1; // the +1 makes some room at the end for the cursor
    height = line.height();
    ascent = line.ascent();
}

void EditionArea::draw(int x, int y, QPainter &painter, bool cursor)
{
    EditionNode::draw(x, y, painter, cursor);
    QRect brect = QRect(x, y, width, height);

    QTextLayout layout(text, painter.font());
    layout.beginLayout();
    auto line = layout.createLine();
    layout.endLayout();
    layout.draw(&painter, brect.topLeft());
    mCursorCoords = QPoint(getPos().x() + line.cursorToX(&cursor_pos), getPos().y() + line.ascent());

    if (cursor) {
        layout.drawCursor(&painter, brect.topLeft(), cursor_pos, 1);
    }
}

QPoint EditionArea::getCursorCoordinates() const
{
    return mCursorCoords;
}
