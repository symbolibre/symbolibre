#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

#include "fracrenderer.h"

class Button;

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void digitON();

    void lparON();
    void rparON();

    void unaryopON();
    void addopON();
    void subopON();
    void mulopON();
    void fracopON();

    void backspaceON();
    void clearON();

    void upON();
    void downON();
    void rightON();
    void leftON();

private:
    Button *createButton(const QString &text, const char *member);

    FracRenderer *display;

    enum { NumDigitButtons = 10 };
    Button *digitButtons[NumDigitButtons];
};

#endif
