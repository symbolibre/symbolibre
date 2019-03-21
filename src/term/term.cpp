#include <QApplication>
#include <qtermwidget5/qtermwidget.h>

int main (int argc, char **argv)
{
    if (argc != 3)
        return 1;

    QApplication app(argc, argv);

    QTermWidget term (false, nullptr);
    term.resize(320, 200);
    term.setScrollBarPosition(QTermWidget::NoScrollBar);

    term.setShellProgram(argv[1]);
    term.setArgs(QStringList(QString(argv[2])));
    term.startShellProgram();

    term.show();
    
    QObject::connect(&term, &QTermWidget::finished, &term, &QTermWidget::close);

    return app.exec();
}
