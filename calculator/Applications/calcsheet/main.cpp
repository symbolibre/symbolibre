#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include "MathContext.hpp"
#include "keycode.hpp"
#include "VirtualKeyboardContext.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<ETBox>("etBox", 1, 0, "ETBox");
    KeyCode::declareQML();
    VirtualKeyboardContext vk;
    MathContext math;

    QQmlEngine engine;
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("keyboard", &vk);
    context->setContextProperty("math", &math);

    vk.setActive(argc >= 2 && strcmp(argv[1], "-keyboard") == 0);

    QQmlComponent component(&engine, "qrc:/main.qml");
    if (component.status() != QQmlComponent::Ready) {
        qCritical() << component.errors();
        return 1;
    }
    auto *window = component.create(context);
    context->setParent(window);

    return app.exec();
}
