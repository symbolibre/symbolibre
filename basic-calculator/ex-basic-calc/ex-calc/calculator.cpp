#include <QtWidgets>

#include <cmath>

#include "button.h"
#include "calculator.h"

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    display = new FracRenderer(); /* TODO : entry */

    for (int i = 0; i < NumDigitButtons; i++)
        digitButtons[i] = createButton(QString::number(i), SLOT(digitON()));

    Button *backspaceButton = createButton(tr("⌫"), SLOT(backspaceON()));
    Button *clearButton     = createButton(tr("Clear"), SLOT(clearON()));

    /* Buttons : parenthesis */
    Button *lparenButton    = createButton(tr("("), SLOT(lparON()));
    Button *rparenButton    = createButton(tr(")"), SLOT(rparON()));

    /* Buttons : operators */
    Button *fracButton    = createButton(tr("―"), SLOT(fracopON()));
    Button *timesButton   = createButton(tr("\303\227"), SLOT(mulopON()));
    Button *minusButton   = createButton(tr("-"), SLOT(subopON()));
    Button *plusButton    = createButton(tr("+"), SLOT(addopON()));

    /* Buttons : arrows */
    Button *leftButton    = createButton(tr("←"), SLOT(leftON()));
    Button *rightButton   = createButton(tr("→"), SLOT(rightON()));
    Button *upButton      = createButton(tr("↑"), SLOT(upON()));
    Button *downButton    = createButton(tr("↓"), SLOT(downON()));

    /* -------- Grid -------- */
    QGridLayout *keyboardLayout = new QGridLayout;
    keyboardLayout->setSizeConstraint(QLayout::SetFixedSize);
    keyboardLayout->addWidget(display,         0, 0, 1, 6);
    keyboardLayout->addWidget(backspaceButton, 1, 4, 1, 1);
    keyboardLayout->addWidget(clearButton,     2, 4, 1, 1);

    /* Arrows buttons */
    keyboardLayout->addWidget(upButton,        1, 2, 1, 1);
    keyboardLayout->addWidget(leftButton,      2, 1, 1, 1);
    keyboardLayout->addWidget(downButton,      2, 2, 1, 1);
    keyboardLayout->addWidget(rightButton,     2, 3, 1, 1);

    for (int i = 1; i < NumDigitButtons; ++i) {
        int row    = ((9 - i) / 3) + 3;
        int column = ((i - 1) % 3) + 1;
        keyboardLayout->addWidget(digitButtons[i], row, column);
    }

    keyboardLayout->addWidget(lparenButton, 6, 1);
    keyboardLayout->addWidget(digitButtons[0], 6, 2);
    keyboardLayout->addWidget(rparenButton, 6, 3);

    keyboardLayout->addWidget(fracButton,  3, 4);
    keyboardLayout->addWidget(timesButton, 4, 4);
    keyboardLayout->addWidget(minusButton, 5, 4);
    keyboardLayout->addWidget(plusButton,  6, 4);

    setLayout(keyboardLayout);

    QAction *action_quit = new QAction(this);
    action_quit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(action_quit, &QAction::triggered, this, &Calculator::close);
    addAction(action_quit);

    setWindowTitle(tr("Demo"));
}

void Calculator::digitON()
{
    Button *cbutton = qobject_cast<Button *>(sender());
    int value = cbutton->text().toInt();
    display->recvDigit(value);
}

void Calculator::lparON()
{
    display->recvLParen();
}
void Calculator::rparON()
{
    display->recvRParen();
}

void Calculator::addopON()
{
    display->recvPlus();
}

void Calculator::subopON()
{
    display->recvMinus();
}

void Calculator::mulopON()
{
    display->recvTimes();
}

void Calculator::fracopON()
{
    display->recvFrac();
}

void Calculator::backspaceON()
{
    display->recvDelete();
}

void Calculator::clearON()
{
    display->recvClear();
}

void Calculator::upON()
{
    display->recvArrow(UP);
}
void Calculator::downON()
{
    display->recvArrow(DOWN);
}
void Calculator::rightON()
{
    display->recvArrow(RIGHT);
}
void Calculator::leftON()
{
    display->recvArrow(LEFT);
}

Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}