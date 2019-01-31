#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include <stdio.h> /* TODO : cpp way */
#include <unistd.h>

#include "calcsheet.hpp"
#include "keycode.hpp"

int main(int argc, char *argv[])
{
    int in[2];
    int out[2];
    if (pipe(in))  {
        fprintf(stderr, "Pipe failed\n");
        exit(1);
    }
    if (pipe(out)) {
        fprintf(stderr, "Pipe failed\n");
        exit(1);
    };

    if (fork()) {
        QGuiApplication app(argc, argv);

        close(in[0]);
        close(out[1]);
        dup2(in[1], STDOUT_FILENO);
        dup2(out[0], STDIN_FILENO);

        qmlRegisterType<CalcSheet>("io.qt.symbolibre.mathrenderer", 1, 0, "MathRenderer");
        KeyCode::declareQML();

        QQmlApplicationEngine engine;
        engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
        if (engine.rootObjects().isEmpty())
            return -1;

        int ret_value = app.exec();
        printf("quit\n");
        return ret_value;
    }

    else {
        close(in[1]);
        close(out[0]);
        if (dup2(in[0], STDIN_FILENO))
            fprintf(stderr, "Error when trying move stdin\n");
        dup2(out[1], STDOUT_FILENO);

        execlp("sage", "-python", "sage_shell.py", (char *) NULL);
    }

    return 0;
}
