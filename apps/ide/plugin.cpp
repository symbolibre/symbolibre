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
        qmlRegisterType<SourceDocument>(uri, 0, 2, "SourceDocument");
        qmlRegisterType<LanguageData>(uri, 0, 2, "LanguageData");
    }
};

#include "plugin.moc"
