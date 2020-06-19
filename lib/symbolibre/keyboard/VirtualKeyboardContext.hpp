#ifndef VIRTUALKEYBOARDCONTEXT_H
#define VIRTUALKEYBOARDCONTEXT_H

#include <QQmlEngine>

class VirtualKeyboardContext : public QObject
{
    Q_OBJECT

public:
    VirtualKeyboardContext();

    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

    bool isActive() const
    {
        return mActive;
    }

    Q_INVOKABLE void setActive(bool state);

public slots:
    void virtualKeyPress(int key, int modifiers, QString text);

signals:
    void activeChanged(bool enabled);

private:
    bool mActive;
};

#endif // VIRTUALKEYBOARDCONTEXT_H
