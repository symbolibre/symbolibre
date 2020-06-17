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
        /* Letters */
        SLK_A, SLK_B, SLK_C, SLK_D, SLK_E, SLK_F,
        SLK_G, SLK_H, SLK_I, SLK_J, SLK_K, SLK_L,
        SLK_M, SLK_N, SLK_O, SLK_P, SLK_Q, SLK_R,
        SLK_S, SLK_T, SLK_U, SLK_V, SLK_W, SLK_X,
        SLK_Y, SLK_Z,

        /* Small letters */
        SLK_a, SLK_b, SLK_c, SLK_d, SLK_e, SLK_f,
        SLK_g, SLK_h, SLK_i, SLK_j, SLK_k, SLK_l,
        SLK_m, SLK_n, SLK_o, SLK_p, SLK_q, SLK_r,
        SLK_s, SLK_t, SLK_u, SLK_v, SLK_w, SLK_x,
        SLK_y, SLK_z,

        /* Digits */
        SLK_0, SLK_1, SLK_2, SLK_3, SLK_4, SLK_5,
        SLK_6, SLK_7, SLK_8, SLK_9,

        /* Ponctuation */
        SLK_COMMA, SLK_DOT, SLK_SPACE,

        /* Mathematics operations */
        SLK_PLUS, SLK_MINUS, SLK_TIMES, SLK_DIVIDE,
        SLK_LPAREN, SLK_RPAREN, SLK_SQUAREROOT, SLK_POWER,
        SLK_SIGMA,

        /* Arrow Keys */
        SLK_UP, SLK_DOWN, SLK_LEFT, SLK_RIGHT,

        /* Special keys */
        SLK_SND, SLK_ALPHA, SLK_DEL, SLK_CLEAR,
        SLK_ANS, SLK_COLONEQ, SLK_EXE, SLK_NULL,
        SLK_APPROX,

        /* New keys */ //TODO: dispatch when debugged
        SLK_COS, SLK_SIN, SLK_TAN,
        SLK_ASIN, SLK_ATAN, SLK_ACOS,
        SLK_PI, SLK_SQUARE, SLK_EXP, SLK_LN, SLK_LOG,
        SLK_POW10,
    };
    Q_ENUMS(keycode)

    static void declareQML()
    {
        qmlRegisterType<KeyCode>("SLKeyCode", 1, 0, "KeyCode");
    }

};

#endif // KEYCODE_H
