#include <QApplication>
#include <qtermwidget5/qtermwidget.h>

int main (int argc, char **argv)
{
    QApplication app(argc, argv);

    QTermWidget term (false, nullptr);
    term.resize(320, 200);
    term.setScrollBarPosition(QTermWidget::NoScrollBar);

    if (argc > 1)
        term.setShellProgram(argv[1]);

    QStringList args;
    for (int i = 2; i < argc; ++i) {
        args << argv[i];
    }
    term.setArgs(args);
    term.startShellProgram();

    term.show();
    
    QObject::connect(&term, &QTermWidget::finished, &term, &QTermWidget::close);

    return app.exec();
}
