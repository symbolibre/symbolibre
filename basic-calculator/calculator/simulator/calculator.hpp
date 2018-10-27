#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

#include "fracrenderer.hpp"

class Button;

/* The class Calculator provides a QWidget that looks like
 * a simulation of a calculator. It has two areas:
 * - a screen
 * - a keyboard. */

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void digitPushed();

    void lparPushed();
    void rparPushed();
    void fracopPushed(void);

    void charPushed();

    void backspacePushed();
    void clearPushed();

    void upPushed();
    void downPushed();
    void rightPushed();
    void leftPushed();

private:
    Button *createButton(const QString &text, const char *member);

    FracRenderer *display;

    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
};

#endif
