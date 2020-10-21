// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SLKEY_HPP
#define SLKEY_HPP

#include <QQmlEngine>


class SLKey : public QObject
{
    Q_OBJECT

public:
    enum Keycode
    {
        Null = 0,

        Shift = Qt::Key_Shift, Alpha = Qt::Key_AltGr,

        F1 = Qt::Key_F1, F2 = Qt::Key_F2, F3 = Qt::Key_F3, F4 = Qt::Key_F4, F5 = Qt::Key_F5,

        Home = Qt::Key_F6, Help = Qt::Key_F7, Back = Qt::Key_Escape, Undo = Qt::Key_F8,
        Delete = Qt::Key_Backspace, Enter = Qt::Key_Return,

        Pi = Qt::Key_F9, Imag = Qt::Key_F13,
        Cos = Qt::Key_F14, Sin = Qt::Key_F15, Tan = Qt::Key_F16,
        Square = Qt::Key_F10, Sqrt = Qt::Key_F11,
        LogE = Qt::Key_F17, Log10 = Qt::Key_F18, Exp = Qt::Key_F19, Pow = Qt::Key_F12,

        If = Qt::Key_F20, Else = Qt::Key_F21,
        For = Qt::Key_F22, While = Qt::Key_F23,
        Input = Qt::Key_F24, Print = Qt::Key_F25
    };
    Q_ENUM(Keycode)

    static Q_INVOKABLE QString text(int k);
    static Q_INVOKABLE QString label(int k);
};

#endif // SLKEY_HPP
