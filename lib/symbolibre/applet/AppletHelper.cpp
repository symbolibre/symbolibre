#include "AppletHelper.hpp"

#include <QQuickItem>
#include <QQuickWindow>

#include "FunctionBarAttached.hpp"

AppletHelper::AppletHelper() : QObject(), m_window(nullptr)
{
    m_functionBar = new FunctionBarAttached(this);
}

void AppletHelper::setWindow(QQuickWindow *window)
{
    if (window == m_window)
        return;

    if (m_window)
        disconnect(m_window, &QQuickWindow::activeFocusItemChanged,
            this, &AppletHelper::updateModel);
    if (window)
        connect(window, &QQuickWindow::activeFocusItemChanged,
            this, &AppletHelper::updateModel);

    m_window = window;
    if (m_window) updateModel();
}

void AppletHelper::updateModel()
{
    FunctionKeyModel *previousModel[6];

    for (int key = 1; key <= 5; key++) {
        previousModel[key] = m_functionBar->getF(key);
        m_functionBar->setF(key, nullptr);
    }

    if (!m_window) {
        emit functionBarChanged(nullptr);
        return;
    }

    auto *focusItem = m_window->activeFocusItem();
    while (focusItem) {
        auto *obj = qmlAttachedPropertiesObject<FunctionBarAttached>(focusItem, false);
        auto *attached = qobject_cast<FunctionBarAttached *>(obj);
        if (attached) {
            for (int key = 1; key <= 5; key++) {
                FunctionKeyModel *model = attached->getF(key);
                if (model) m_functionBar->setF(key, model);
            }

            if (!attached->combine()) break;
        }
        focusItem = focusItem->parentItem();
    }

    bool changed = false;
    for (int key = 1; key <= 5; key++) {
        if (m_functionBar->getF(key) != previousModel[key]) changed = true;
    }

    if (changed) emit functionBarChanged(m_functionBar);
}
