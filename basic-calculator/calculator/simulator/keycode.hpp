#ifndef KEYCODE_H
#define KEYCODE_H

#include <QtGlobal>
#include <QQmlEngine>


class KeyCode : public QObject
{
    Q_OBJECT

public:
    KeyCode() : QObject() {}

    enum keycode /* SymboLibreKey */
    {
        /* Letters -- add lower ones ?*/
        SLK_A, SLK_B, SLK_C, SLK_D, SLK_E, SLK_F,
        SLK_G, SLK_H, SLK_I, SLK_J, SLK_K, SLK_L,
        SLK_M, SLK_N, SLK_O, SLK_P, SLK_Q, SLK_R,
        SLK_S, SLK_T, SLK_U, SLK_V, SLK_W, SLK_X,
        SLK_Y, SLK_Z,

        /* Digits */
        SLK_0, SLK_1, SLK_2, SLK_3, SLK_4, SLK_5,
        SLK_6, SLK_7, SLK_8, SLK_9,

        /* Mathematics operations */
        SLK_PLUS, SLK_MINUS, SLK_TIMES, SLK_DIVIDE,
        SLK_LPAREN, SLK_RPAREN, SLK_SQUAREROOT,

        /* Arrow Keys */
        SLK_UP, SLK_DOWN, SLK_LEFT, SLK_RIGHT,

        /* Special keys */
        SLK_SND, SLK_ALPHA, SLK_DEL, SLK_CLEAR,
        SLK_ANS, SLK_EXE, SLK_NULL
    };
    Q_ENUMS(keycode);

    static void declareQML()
    {
        qmlRegisterType<KeyCode>("SLKeyCode", 1, 0, "KeyCode");
    }

};

#endif // KEYCODE_H
