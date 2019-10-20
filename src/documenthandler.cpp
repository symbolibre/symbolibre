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

#include "documenthandler.h"

#include <QFile>
#include <QFileInfo>
#include <QQuickTextDocument>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QDebug>

DocumentHandler::DocumentHandler(QWidget *parent)
    : QObject(parent)
{
    /* Whenever the file path changes, the language model is also supposed to
       change. (Opening new files and saving with new extensions can all
       reasonably change the language.) */
    connect(this, &DocumentHandler::filePathChanged,
            this, &DocumentHandler::docLanguageChanged);

    m_document = nullptr;
    m_cursorPosition = -1;
    m_selectionStart = 0;
    m_selectionEnd = 0;

    /* Overridden by setting the [fontSize] attribute from QML */
    setFontSize(13);

    setDocLanguage(0);
    setLanguageModel(nullptr);

    m_highlighter = nullptr;
    m_process = new Process(parent);
}

void DocumentHandler::setFontSize(int size)
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
        QTextCursor cursor = textCursor();
        if (!cursor.isNull()) {
            cursor.select(QTextCursor::Document);
            QTextCharFormat format;
            format.setFont(f);
            cursor.mergeCharFormat(format);
        }
    }

    emit fontSizeChanged();
}

QString DocumentHandler::langExtension() const
{
    if (m_languageModel == nullptr)
        return QString(".txt");

    return m_languageModel->getExtensionFromId(docLanguage());
}

QString DocumentHandler::filePath() const
{
    return m_filePath;
}

int DocumentHandler::docLanguage() const
{
    return m_docLanguage;
}

void DocumentHandler::setDocLanguage(int lang)
{
    if (lang == docLanguage())
        return;

    m_docLanguage = lang;
    emit docLanguageChanged();
    emit filePathChanged();

    return;
}

void DocumentHandler::setDocLanguageFromExtension(QString fileExt)
{
    int newDocLanguage = 0;

    newDocLanguage = m_languageModel->getIdFromExtension(fileExt);

    setDocLanguage(newDocLanguage);
}


/* Returns the name required by the KSyntaxHighlighter to correctly identify
   our file type, and to use the proper syntax KSyntaxHighlighting::Definition
   to highlight it. */
QString DocumentHandler::syntaxDefinitionName(void) const
{
    return m_languageModel->getColorationFromId(docLanguage());
}

LanguagesModel *DocumentHandler::languageModel() const
{
    return m_languageModel;
}

void DocumentHandler::setLanguageModel(LanguagesModel *langModel)
{
    if (m_languageModel == langModel)
        return;

    m_languageModel = langModel;
    emit languageModelChanged();
}


Process *DocumentHandler::process() const
{
    return m_process;
}

void DocumentHandler::setProcess(Process *newProcess)
{
    if (m_process == newProcess)
        return;

    m_process = newProcess;
    emit processChanged();
}

snippetMap_t DocumentHandler::snippets()
{
    return m_languageModel->getSnippetsFromId(docLanguage());
}

void DocumentHandler::setSnippets(snippetMap_t)
{
    /* TODO: DocumentHandler::setSnippets() probably shouldn't be empty */
}

int DocumentHandler::insertSnippet(QString key)
{
    /* TODO: If the selection is non-empty, it is correctly deleted but the
       TODO: cursor is placed after the snippet instead of in the middle. */

    QString snipp = ((m_languageModel->getSnippetsFromId(docLanguage())))[key];
    int old_position = m_cursorPosition;
    int pos_in_snipp = snipp.indexOf("*");
    snipp.remove("*");

    textCursor().insertText(snipp);
    if (pos_in_snipp > -1)
        m_cursorPosition = old_position + pos_in_snipp;

    return m_cursorPosition;
}

void DocumentHandler::execute()
{
    if (m_process == nullptr)
        return;

    /* TODO: Save the file to execute somewhere else (or pipe it) */
    QFile tempfile("temp");
    if (!tempfile.open(QFile::ReadWrite | QFile::Truncate | QFile::Text))
           return;

    QTextStream out(&tempfile);
    out << textDocument()->toPlainText();
    tempfile.close();

    QString cmd = languageModel()->getCmdFromId(docLanguage());
    QString file = m_filePath;
    QStringList args;
    args << cmd << "./temp";

    m_process->start("./term/term", args);
}

void DocumentHandler::load(const QString &filePath)
{
    QString local = filePath;

    if (filePath.startsWith("file://"))
        local = filePath.right(filePath.size() - 7);

    if (QFile::exists(local)) {
        QFile file(local);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            QTextStream in(&file);
            QString data = in.readAll();
            if (QTextDocument *doc = textDocument())
                doc->setModified(false);

            emit loaded(data.toUtf8());
        }
    }

    m_filePath = local;
    emit filePathChanged();

    setDocLanguageFromExtension(QFileInfo(local).suffix());

    const QString defName = syntaxDefinitionName();

    const auto def = m_repository.definitionForName(defName);
    if (def.isValid()) {
            m_highlighter->setDefinition(def);
            m_highlighter->rehighlight();
    }
    else {
        qInfo() << "Invalid syntax highlighting definition: '" << defName
            << "' not found\n";
    }
}

void DocumentHandler::startHighlighter(void){

    m_repository.addCustomSearchPath("syntax-highlighting/data");
    m_repository.addCustomSearchPath("syntax-files");
    m_repository.addCustomSearchPath("../src/syntax-files");
    m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(textDocument());

    const QString defName = syntaxDefinitionName();

    const auto def = m_repository.definitionForName(defName);
    if (!def.isValid()){
        qInfo() << "Definition for syntax highlighting is not valid : the name " << defName << " was not found\n";
        return;
    }

    const auto theme = m_repository.theme("Solarized Light");
    if (!theme.isValid()){
        qInfo() << "Theme for syntax highlighting is not valid\n";
        return;
    }

    m_highlighter->setDefinition(def);

    m_highlighter->setTheme(theme);
    m_highlighter->rehighlight();
}

void DocumentHandler::saveAs(const QString &filePath)
{
    QString local = filePath;

    if (filePath.startsWith("file://"))
        local = filePath.right(filePath.size() - 7);

    QTextDocument *doc = textDocument();
    if (!doc) return;

    QFile file(local);

    if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((doc->toPlainText()).toUtf8());
    file.close();

    if (filePath == m_filePath) return;

    m_filePath = filePath;
    emit filePathChanged();
}

bool DocumentHandler::wasAlreadySaved(void)
{
    return !filePath().isEmpty();
}

QTextCursor DocumentHandler::textCursor() const
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

QTextDocument *DocumentHandler::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}
