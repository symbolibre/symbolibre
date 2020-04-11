#include "VirtualKeyboardContext.hpp"

#include <QGuiApplication>
#include <QKeyEvent>
#include <QWindow>

VirtualKeyboardContext::VirtualKeyboardContext() :
    QObject(), mActive(false)
{

}

void VirtualKeyboardContext::setActive(bool state)
{
    mActive = state;
    emit activeChanged(state);
}

void VirtualKeyboardContext::virtualKeyPress(int key, int modifiers, QString text)
{
    QKeyEvent pressEvent(QEvent::KeyPress, key, Qt::KeyboardModifier(modifiers), text);
    QKeyEvent releaseEvent(QEvent::KeyRelease, key, Qt::KeyboardModifier(modifiers), text);
    static_cast<QObject *>(QGuiApplication::focusWindow())->event(&pressEvent);
    static_cast<QObject *>(QGuiApplication::focusWindow())->event(&releaseEvent);
}
