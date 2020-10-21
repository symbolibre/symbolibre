// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SL_EDITIONTREE_FONTRESIZER_HPP
#define SL_EDITIONTREE_FONTRESIZER_HPP

#include <QFont>

class QPainter;

/**
 * This tiny RAII helper temporarily changes the point size of the font of a QPainter
 */
class FontResizer
{
public:
    FontResizer(QPainter &painter, int pointSize);
    ~FontResizer();

private:
    QPainter *mPainter;
    QFont mFont;
};

#endif // SL_EDITIONTREE_FONTRESIZER_HPP
