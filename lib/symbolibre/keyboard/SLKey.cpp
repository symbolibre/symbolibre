// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SLKey.hpp"

QString SLKey::text(int k)
{
    switch (k) {
    case Null:
        return "???";

    case Enter:
        return "\n";

    case Pi:
        return "\u03C0";

    case Imag:
        return "i";

    case Euler:
        return "e";

    case Cos:
        return "cos";

    case Sin:
        return "sin";

    case Tan:
        return "tan";

    case Square:
        return "\u00B2";

    case Sqrt:
        return "\u221A";

    case LogE:
        return "ln";

    case Log10:
        return "log";

    case Exp:
        return "e^";//"\u212F^";

    case Pow:
        return "^";

    case If:
        return "if";

    case Else:
        return "else";

    case For:
        return "for";

    case While:
        return "while";

    case Input:
        return "input";

    case Print:
        return "print";
    }

    // ASCII
    if (k >= ' ' && k <= '~')
        return QChar(k);

    return QString();
}

QString SLKey::label(int k)
{

    switch (k) {
    case Null:
        return "???";

    case Shift:
        return "\u21E7";

    case Alpha:
        return "Alpha";

    case Home:
        return "Home";//"\u2302";

    case Help:
        return "Help";

    case Back:
        return "Back";

    case Undo:
        return "Undo";

    case Delete:
        return "\u232B";

    case Enter:
        return "Enter";

    case Qt::Key_Space:
        return "\u2423";
    }

    if (k >= Qt::Key_Left && k <= Qt::Key_Down)
        return QChar(0x2190 + k - Qt::Key_Left);

    if (k >= F1 && k <= F5)
        return QString("F") + QChar(k - F1 + '1');

    QString ret = text(k);
    if (!ret.isEmpty())
       return ret;

    return "???";
}
