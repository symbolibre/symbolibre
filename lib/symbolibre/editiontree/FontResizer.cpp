#include "FontResizer.hpp"

#include <QPainter>

FontResizer::FontResizer(QPainter &painter, int pointSize) :
    mPainter(&painter), mFont(painter.font())
{
    auto font = painter.font();
    font.setPointSize(std::max(pointSize, 8));
    painter.setFont(font);
    if (painter.fontInfo().pixelSize() < 8) {
        font.setPixelSize(8);
        painter.setFont(font);
    }
}

FontResizer::~FontResizer()
{
    mPainter->setFont(mFont);
}
