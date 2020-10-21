// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QQmlExtensionPlugin>
#include "SLKey.hpp"
#include "VirtualKeyboardContext.hpp"

class GraphQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.keyboard")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.keyboard"));
        qmlRegisterSingletonType<SLKey>(uri, 1, 0, "SLKey",
            [](QQmlEngine *, QJSEngine *) {
                return new SLKey;
            });
        qmlRegisterType<VirtualKeyboardContext>("org.symbolibre.keyboard", 1, 0,
                                                "VirtualKeyboardContext");
    }
};

#include "plugin.moc"
