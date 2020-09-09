/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "SourceEditor.h"

#include <QFile>
#include <QFileInfo>
#include <QQuickTextDocument>
#include <QTemporaryFile>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QDebug>
#include <qtermwidget5/qtermwidget.h>

#include <symbolibre/util/FileSystemSingleton.hpp>

SourceEditor::SourceEditor(QWidget *parent)
    : QObject(parent)
{
    m_document = nullptr;
    m_cursorPosition = -1;
    m_selectionStart = 0;
    m_selectionEnd = 0;

    /* Overridden by setting the [fontSize] attribute from QML */
    setFontSize(13);

    m_languageData = &m_languages.getLanguageFromName("Plain text");

    m_repository.addCustomSearchPath(Fs::staticDataDir() + "/ide/syntax-files");

    m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(this);
    const auto theme = m_repository.theme("Solarized Light");
    if (!theme.isValid()){
        qInfo() << "Theme for syntax highlighting is not valid\n";
        return;
    }
    m_highlighter->setTheme(theme);
    connect(this, &SourceEditor::documentChanged, [&]() {
        m_highlighter->setDocument(textDocument());
    });
}

void SourceEditor::setFontSize(int size)
{
    if (size <= 0) return;
    m_fontSize = size;

    /* Set the font size on the text edition area */

    QTextDocument *doc = textDocument();
    if (doc) {
        QFont f = doc->defaultFont();
        f.setPixelSize(size);
        doc->setDefaultFont(f);

        /* The text document has a local formatting style (like rich text).
           Select the whole document then set the font size. */
        QTextCursor cursor(doc);
        if (!cursor.isNull()) {
            cursor.select(QTextCursor::Document);
            QTextCharFormat format;
            format.setFont(f);
            cursor.mergeCharFormat(format);
        }
    }

    emit fontSizeChanged();
}

LanguageData *SourceEditor::languageData() const
{
    return m_languageData;
}

QString SourceEditor::filePath() const
{
    return m_filePath;
}

int SourceEditor::insertSnippet(QString snippet)
{
    /* TODO: If the selection is non-empty, it is correctly deleted but the
       TODO: cursor is placed after the snippet instead of in the middle. */

    int old_position = m_cursorPosition;
    int pos_in_snipp = snippet.indexOf("*");
    snippet.remove("*");

    textCursor().insertText(snippet);
    if (pos_in_snipp > -1)
        m_cursorPosition = old_position + pos_in_snipp;

    return m_cursorPosition;
}

void SourceEditor::execute()
{
    // We create a Qt Widgets window for the terminal
    // FIXME we should set an appropriate font
    auto *term = new QTermWidget(false, nullptr);
    connect(term, &QTermWidget::finished, term, &QTermWidget::close);
    term->setAttribute(Qt::WA_DeleteOnClose);
    term->resize(320, 222);
    term->setScrollBarPosition(QTermWidget::NoScrollBar);

    auto *tempFile = new QTemporaryFile("symbolibre-ide", term);
    if (!tempFile->open())
       return;
    QTextStream out(tempFile);
    out << textDocument()->toRawText();
    tempFile->close();

    term->setShellProgram(m_languageData->command);
    term->setArgs(QStringList() << tempFile->fileName());
    term->startShellProgram();

    term->show();
}

void SourceEditor::load(const QString &filePath)
{
    QString local = filePath;

    if (filePath.startsWith("file://"))
        local = filePath.right(filePath.size() - 7);

    QFile file(local);

    if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
        QString data(file.readAll());
        QTextDocument *doc = textDocument();
        if (doc) doc->setModified(false);
        emit loaded(data.toUtf8());
    }

    m_filePath = local;
    emit filePathChanged();

    const auto def = m_repository.definitionForFileName(filePath);
    m_highlighter->setDefinition(def);
    m_highlighter->rehighlight();
    if (def.isValid()) {
        m_languageData = &m_languages.getLanguageFromName(def.name());
    } else {
        m_languageData = &m_languages.getLanguageFromName("Text");
    }
    emit languageDataChanged(m_languageData);
}

void SourceEditor::saveAs(const QString &filePath)
{
    QString local = filePath;

    if (filePath.startsWith("file://"))
        local = filePath.right(filePath.size() - 7);

    QTextDocument *doc = textDocument();
    if (!doc) return;

    QFile file(local);

    if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        emit error(file.errorString());
        return;
    }
    file.write((doc->toPlainText()).toUtf8());
    file.close();

    if (filePath == m_filePath) return;

    m_filePath = filePath;
    emit filePathChanged();
}

QTextCursor SourceEditor::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

QTextDocument *SourceEditor::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}
