#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include "FileSystemContext.hpp"
#include "MathContext.hpp"
#include "keyboard/keycode.hpp"
#include "keyboard/VirtualKeyboardContext.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ETBox>("etBox", 1, 0, "ETBox");
    KeyCode::declareQML();
    VirtualKeyboardContext vk;
    MathContext math;
    FileSystemContext fs;

    QQmlEngine engine;
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("keyboard", &vk);
    context->setContextProperty("math", &math);
    context->setContextProperty("fs", &fs);

    vk.setActive(argc >= 2 && strcmp(argv[1], "-keyboard") == 0);

    QQmlComponent component(&engine, QUrl::fromLocalFile("./main.qml"));
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}
