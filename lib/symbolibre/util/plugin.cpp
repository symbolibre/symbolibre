// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FileSystemSingleton.hpp"
#include <QQmlExtensionPlugin>

class UtilQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.util")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.util"));
        qmlRegisterSingletonType<FileSystemSingleton>(uri, 0, 2, "Fs",
        [](QQmlEngine *, QJSEngine *) -> QObject* {
            return new FileSystemSingleton;
        }
                                                     );
    }
};

#include "plugin.moc"
