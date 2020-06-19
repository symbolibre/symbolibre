#include "FontResizer.hpp"

#include <QPainter>

FontResizer::FontResizer(QPainter &painter, int pointSize) :
    mPainter(&painter), mFont(painter.font())
{
    auto font = painter.font();
    font.setPointSize(pointSize);
    painter.setFont(font);
}

FontResizer::~FontResizer()
{
    mPainter->setFont(mFont);
}
