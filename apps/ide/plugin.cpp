// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SourceDocument.hpp"
#include <QQmlExtensionPlugin>

class EditorQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.symbolibre.editor")
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(uri == QString("org.symbolibre.editor"));
        qmlRegisterType<SourceDocument>(uri, 1, 0, "SourceDocument");
        qmlRegisterType<LanguageData>(uri, 1, 0, "LanguageData");
    }
};

#include "plugin.moc"
