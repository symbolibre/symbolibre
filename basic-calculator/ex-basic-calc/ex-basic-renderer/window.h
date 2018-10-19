#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class PrettyPrinter;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(const char *top, const char *bot);

private:
    PrettyPrinter *render;
};

#endif // WINDOW_H
