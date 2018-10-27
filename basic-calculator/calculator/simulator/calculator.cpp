#include <QtWidgets>

#include <cmath>

#include "button.hpp"
#include "calculator.hpp"

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    display = new FracRenderer(); /* TODO : entry */

    for (int i = 0; i < NumDigitButtons; i++)
        digitButtons[i] = createButton(QString::number(i), SLOT(digitPushed()));

    Button *backspaceButton = createButton(tr("⌫"), SLOT(backspacePushed()));
    Button *clearButton     = createButton(tr("Clear"), SLOT(clearPushed()));

    /* Buttons : parenthesis */
    Button *lparenButton    = createButton(tr("("), SLOT(lparPushed()));
    Button *rparenButton    = createButton(tr(")"), SLOT(rparPushed()));

    /* Buttons : operators */
    Button *fracButton    = createButton(tr("―"), SLOT(fracopPushed()));
    Button *timesButton   = createButton(tr("*"), SLOT(charPushed()));
    Button *minusButton   = createButton(tr("-"), SLOT(charPushed()));
    Button *plusButton    = createButton(tr("+"), SLOT(charPushed()));

    /* Buttons : arrows */
    Button *leftButton    = createButton(tr("←"), SLOT(leftPushed()));
    Button *rightButton   = createButton(tr("→"), SLOT(rightPushed()));
    Button *upButton      = createButton(tr("↑"), SLOT(upPushed()));
    Button *downButton    = createButton(tr("↓"), SLOT(downPushed()));

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

    setWindowTitle(tr("Calculator"));
}

void Calculator::digitPushed()
{
    Button *cbutton = qobject_cast<Button *>(sender());
    int value = cbutton->text().toInt();
    display->recvDigit(value);
}

void Calculator::lparPushed()
{
    display->recvLParen();
}

void Calculator::rparPushed()
{
    display->recvRParen();
}

void Calculator::charPushed()
{
    Button *cbutton = qobject_cast<Button *>(sender());
    char c = cbutton->text().at(0).toLatin1();
    display->recvChar(c);
}

void Calculator::fracopPushed()
{
    display->recvFrac();
}

void Calculator::backspacePushed()
{
    display->recvDelete();
}

void Calculator::clearPushed()
{
    display->recvClear();
}

void Calculator::upPushed()
{
    display->recvArrow(UP);
}

void Calculator::downPushed()
{
    display->recvArrow(DOWN);
}

void Calculator::rightPushed()
{
    display->recvArrow(RIGHT);
}

void Calculator::leftPushed()
{
    display->recvArrow(LEFT);
}

Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}
