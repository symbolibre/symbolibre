#include "prettyprinter.h"
#include "window.h"

#include <QtWidgets>

const int IdRole = Qt::UserRole;

Window::Window(const char *top, const char *bot)
{
    render = new PrettyPrinter(tr(top), tr(bot));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setColumnStretch(0, 1);
    mainLayout->addWidget(render, 0, 0, 1, 0);
    setLayout(mainLayout);

    setWindowTitle(tr("Pretty Printer"));
}

