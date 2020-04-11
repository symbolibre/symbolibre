#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>

#include "calcsheet.hpp"
#include "keycode.hpp"
#include "VirtualKeyboardContext.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CalcSheet>("org.symbolibre.mathrenderer", 1, 0, "MathRenderer");
    KeyCode::declareQML();
    VirtualKeyboardContext vk;

    QQmlEngine engine;
    auto *context = new QQmlContext(engine.rootContext());
    context->setContextProperty("keyboard", &vk);

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
