#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <iostream>
#include <unistd.h>

#include "calcsheet.hpp"
#include "keycode.hpp"

int main(int argc, char *argv[])
{
    int in[2];
    int out[2];
    if (pipe(in))  {
        std::cerr << "Pipe failed\n";
        return 1;
    }
    if (pipe(out)) {
        std::cerr << "Pipe failed\n";
        return 1;
    }

    if (fork()) {
        QGuiApplication app(argc, argv);

        close(in[0]);
        close(out[1]);
        dup2(in[1], STDOUT_FILENO);
        dup2(out[0], STDIN_FILENO);

        qmlRegisterType<CalcSheet>("org.symbolibre.mathrenderer", 1, 0, "MathRenderer");
        KeyCode::declareQML();

        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

        int ret_value = app.exec();
        std::cout << "quit\n";
        return ret_value;
    }

    else {
        close(in[1]);
        close(out[0]);
        if (dup2(in[0], STDIN_FILENO))
            std::cerr << "Error when trying move stdin\n";
        dup2(out[1], STDOUT_FILENO);

        execlp("sage", "-python", "sage_shell.py", (char *) NULL);
    }

    return 0;
}
