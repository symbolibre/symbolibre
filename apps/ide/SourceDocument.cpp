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

#include "SourceDocument.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QQuickTextDocument>
#include <QTemporaryFile>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QDebug>
#include <qtermwidget5/qtermwidget.h>

#include <symbolibre/util/FileSystemSingleton.hpp>

SourceDocument::SourceDocument(QWidget *parent)
    : QObject(parent), m_term(nullptr), m_executionFinished(false)
{
    m_document = nullptr;
    m_cursorPosition = -1;
    m_selectionStart = 0;
    m_selectionEnd = 0;

    /* Overridden by setting the [fontSize] attribute from QML */
    setFontSize(13);

    m_languageData = m_languages.data("Plain text");

    m_repository.addCustomSearchPath(Fs::staticDataDir() + "/ide/syntax-files");

    m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(this);
    const auto theme = m_repository.theme("Solarized Light");
    if (!theme.isValid()){
        qInfo() << "Theme for syntax highlighting is not valid\n";
        return;
    }
    m_highlighter->setTheme(theme);
    connect(this, &SourceDocument::documentChanged, [&]() {
        m_highlighter->setDocument(textDocument());

        // Propagate the document's modified signal to ours
        connect(textDocument(), &QTextDocument::modificationChanged, this,
            &SourceDocument::modifiedChanged);
    });

    emit languagesChanged(&m_languages);
}

void SourceDocument::setModified(bool modified)
{
    textDocument()->setModified(modified);
}

void SourceDocument::setFontSize(int size)
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

bool SourceDocument::modified() const
{
    if (!textDocument())
        return false;

    return textDocument()->isModified();
}

LanguageData *SourceDocument::languageData() const
{
    return m_languageData;
}

LanguagesModel *SourceDocument::languages()
{
    return &m_languages;
}

QString SourceDocument::filePath() const
{
    return m_filePath;
}

int SourceDocument::insertSnippet(QString escaped_snippet)
{
    /* TODO: If the selection is non-empty, it is correctly deleted but the
       TODO: cursor is placed after the snippet instead of in the middle. */

    int old_position = m_cursorPosition;
    int pos_in_snipp = -1;

    /* Set pos_in_snipp to the first occurrence of unescaped "*", remove other
       unescaped "*", and expand escaped "\*" */
    QString snippet;

    for (auto it = escaped_snippet.begin(); it != escaped_snippet.end(); it++)
    {
        if (*it == '\\') {
            ++it;
            if (it != escaped_snippet.end())
                snippet += *it;
        }
        else if (*it == '*') {
            if (pos_in_snipp < 0)
                pos_in_snipp = it - escaped_snippet.begin();
        }
        else {
            snippet += *it;
        }
    }

    textCursor().insertText(snippet);
    if (pos_in_snipp > -1)
        m_cursorPosition = old_position + pos_in_snipp;

    return m_cursorPosition;
}

int SourceDocument::insertAutoIndent()
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return m_cursorPosition;

    QTextCursor start = textCursor();
    start.movePosition(QTextCursor::StartOfLine);

    QTextCursor end = start;
    while (end.position() < m_cursorPosition &&
        doc->characterAt(end.position()).isSpace()) {
        end.setPosition(end.position() + 1);
    }

    start.setPosition(end.position(), QTextCursor::KeepAnchor);

    textCursor().insertText("\n" + start.selectedText());
    return m_cursorPosition;
}

void SourceDocument::execute()
{
    // Only ever try if there is a command for the current language
    if (m_languageData->command == "")
        return;

    // We create a Qt Widgets window for the terminal
    if (!m_term)
    {
        m_term = new QTermWidget(false, nullptr);

        connect(m_term, &QTermWidget::titleChanged, this, [this]() {
            m_executionFinished = true;
            m_term->sendText(tr("<Finished>"));
        });

        connect(m_term, &QTermWidget::termKeyPressed, this, [this]() {
            if (!m_executionFinished)
                return;

            m_termInput->close();
            m_term->close();
            m_term = nullptr;
            m_termInput = nullptr;
        });
    }

    // FIXME set a font selected from SLStyle
    QFont font("DejaVu Sans Mono");
    font.setStyleHint(QFont::Monospace);
    m_term->setFont(font);
    m_term->setTerminalFont(font);

    m_term->setAttribute(Qt::WA_DeleteOnClose);
    m_term->resize(320, 222);
    m_term->setScrollBarPosition(QTermWidget::NoScrollBar);
    m_term->setAutoClose(false);

    m_termInput = new QTemporaryFile("symbolibre-ide", m_term);
    if (!m_termInput->open())
        return;
    m_termInput->write(textDocument()->toPlainText().toUtf8());
    m_termInput->flush();

    m_executionFinished = false;
    m_term->setShellProgram(m_languageData->command);
    m_term->setArgs(QStringList() << m_termInput->fileName());
    m_term->startShellProgram();
    m_term->show();
}

void SourceDocument::load(const QString &filePath, bool create)
{
    // Resolve absolute vs local path relative to working directory
    QFileInfo info(QDir(m_workingDirectory), filePath);
    QString path = info.absoluteFilePath();
    QFile file(path);

    if (!file.exists()) {
        if (!create)
            return;

        file.open(QFile::WriteOnly);
        file.close();
    }

    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString data(file.readAll());
        QTextDocument *doc = textDocument();
        if (doc) doc->setModified(false);
        emit loaded(data.toUtf8());
    }

    m_filePath = path;
    emit filePathChanged();

    const auto def = m_repository.definitionForFileName(filePath);
    m_highlighter->setDefinition(def);
    m_highlighter->rehighlight();
    if (def.isValid()) {
        m_languageData = m_languages.data(def.name());
    } else {
        m_languageData = m_languages.data("Plain text");
    }
    emit languageDataChanged(m_languageData);
}

void SourceDocument::create(const QString &filePath)
{
    load(filePath, true);
}

void SourceDocument::saveAs(const QString &filePath)
{
    // Resolve absolute vs local path relative to working directory
    QFileInfo info(QDir(m_workingDirectory), filePath);
    QString path = info.absoluteFilePath();
    QFile file(path);

    QTextDocument *doc = textDocument();
    if (!doc) return;

    if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        emit error(file.errorString());
        return;
    }
    file.write((doc->toPlainText()).toUtf8());
    file.close();

    doc->setModified(false);
    emit modifiedChanged();

    if (path == m_filePath) return;

    m_filePath = filePath;
    emit filePathChanged();
}

QTextCursor SourceDocument::textCursor() const
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

QTextDocument *SourceDocument::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}
