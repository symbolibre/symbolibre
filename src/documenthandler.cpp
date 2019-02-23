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
#include <QFileSelector>
#include <QQmlFile>
#include <QQmlFileSelector>
#include <QQuickTextDocument>
#include <QTextCharFormat>
#include <QTextDocument>
#include <QDebug>

DocumentHandler::DocumentHandler(QWidget *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
    , m_docLanguage(TEXT_FILES)
{
}


QQuickTextDocument *DocumentHandler::document() const
{
    return m_document;
}

void DocumentHandler::setDocument(QQuickTextDocument *document)
{
    if (document == m_document)
        return;

    m_document = document;
    emit documentChanged();
}

int DocumentHandler::cursorPosition() const
{
    return m_cursorPosition;
}

void DocumentHandler::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;
    reset();
    emit cursorPositionChanged();
}

int DocumentHandler::selectionStart() const
{
    return m_selectionStart;
}

void DocumentHandler::setSelectionStart(int position)
{
    if (position == m_selectionStart)
        return;

    m_selectionStart = position;
    emit selectionStartChanged();
}

int DocumentHandler::selectionEnd() const
{
    return m_selectionEnd;
}

void DocumentHandler::setSelectionEnd(int position)
{
    if (position == m_selectionEnd)
        return;

    m_selectionEnd = position;
    emit selectionEndChanged();
}


bool DocumentHandler::bold() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontWeight() == QFont::Bold;
}

void DocumentHandler::setBold(bool bold)
{
    QTextCharFormat format;
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(format);
    emit boldChanged();
}

bool DocumentHandler::italic() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontItalic();
}

void DocumentHandler::setItalic(bool italic)
{
    QTextCharFormat format;
    format.setFontItalic(italic);
    mergeFormatOnWordOrSelection(format);
    emit italicChanged();
}

bool DocumentHandler::underline() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontUnderline();
}

void DocumentHandler::setUnderline(bool underline)
{
    QTextCharFormat format;
    format.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(format);
    emit underlineChanged();
}

int DocumentHandler::fontSize() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return 0;
    QTextCharFormat format = cursor.charFormat();
    return format.font().pointSize();
}

void DocumentHandler::setFontSize(int size)
{
    if (size <= 0)
        return;

    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;

    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    if (cursor.charFormat().property(QTextFormat::FontPointSize).toInt() == size)
        return;

    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
    emit fontSizeChanged();
}

QString DocumentHandler::fileName() const
{
    const QString filePath = QQmlFile::urlToLocalFileOrQrc(m_fileUrl);
    const QString fileName = QFileInfo(filePath).fileName();
    if (fileName.isEmpty())
        return QString("Sans titre").append(fileExtension()); //fileExtension is NOT the current extension.
    return fileName;
}

/*
 * This function does NOT return the current extension of the file.
 *  It returns what should be the its extension according to the
 * type of the document that we are currently writing
 * This type is determined by :
 * - user selection
 * - file opening
 * and is stored in the m_docLanguage attribute.
*/
QString DocumentHandler::fileExtension() const
{
    switch (docLanguage()){
        case TEXT_FILES:
            return QString(".txt");
            break;

        case PYTHON_FILES:
            return QString(".py");
            break;

        case OCAML_FILES:
            return QString(".ml");
            break;

        case TI_BASIC_FILES:
            return QString(".tibs");
            break;

        case CASIO_BASIC_FILES:
            return QString(".csbs");
            break;

        default:
            return QString(".txt"); //Might want to return nothing ?
    }
}

QUrl DocumentHandler::fileUrl() const
{
    return m_fileUrl;
}


int DocumentHandler::docLanguage() const
{
    return m_docLanguage;
}


/*
 * This function returns the name required by the KSyntaxHighlighter
 * to correctly identify our file type, and to use the right syntax
 * KSyntaxHighlighting::Definition to highlight it.
*/
QString DocumentHandler::syntaxDefinitionName(void) const
{
    switch(docLanguage()){
        case TEXT_FILES:
            return QString("");
            break;
        case PYTHON_FILES:
            return QString("Python");
            break;
        case OCAML_FILES:
            return QString("Objective Caml");
            break;
        case TI_BASIC_FILES:
            return QString("TI Basic");
            break;
        case CASIO_BASIC_FILES:
            return QString("Casio Basic");
            break;
        default:
            return QString("");
            break;
    }
}


void DocumentHandler::setDocLanguage(int lang)
{
    m_docLanguage = lang;
    emit docLanguageChanged();
    emit fileUrlChanged();

    return;
}

void DocumentHandler::setDocLanguageFromExtension(QString fileExt)
{
    int newDocLanguage = 0;

    // Switch on fileExt to determine newDocLanguage
    if (fileExt.compare("txt") ==  0)
        newDocLanguage = TEXT_FILES;
    else if (fileExt.compare("py") ==  0)
        newDocLanguage = PYTHON_FILES;
    else if (fileExt.compare("ml") == 0)
        newDocLanguage = OCAML_FILES;
    else if (fileExt.compare("tibs") == 0)
        newDocLanguage = TI_BASIC_FILES;
    else if (fileExt.compare("csbs") == 0)
        newDocLanguage = CASIO_BASIC_FILES;
    else
        newDocLanguage = OTHER_FILES;

    setDocLanguage(newDocLanguage);
}


void DocumentHandler::load(const QUrl &fileUrl)
{
    if (fileUrl == m_fileUrl)
        return;

    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "load() called before DocumentHandler has QQmlEngine";
        return;
    }

    const QUrl path = QQmlFileSelector::get(engine)->selector()->select(fileUrl);
    const QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
    if (QFile::exists(fileName)) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
            QTextStream in(&file);
            QString data = in.readAll();
            if (QTextDocument *doc = textDocument())
                doc->setModified(false);

            emit loaded(data.toUtf8());
            reset();
        }
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged();

    setDocLanguageFromExtension(QFileInfo(fileName).suffix());

    //TODO : change with m_highlighter->setDefinition() the file type that we're highlighting
    const QString defName = syntaxDefinitionName();

    const auto def = m_repository.definitionForName(defName);
    if (!def.isValid()){
        qInfo() << "Definition for syntax highlighting is not valid\n";
        return;
    }

    m_highlighter->setDefinition(def);
    m_highlighter->rehighlight();

}

void DocumentHandler::startHighlighter(void){

    //m_repository.addCustomSearchPath("syntax-highlighting/data");
    m_highlighter = new KSyntaxHighlighting::SyntaxHighlighter(document()->textDocument());
    const QString defName = syntaxDefinitionName();

    const auto def = m_repository.definitionForName(defName);
    if (!def.isValid()){
        qInfo() << "Definition for syntax highlighting is not valid\n";
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

void DocumentHandler::saveAs(const QUrl &fileUrl)
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    const QString filePath = fileUrl.toLocalFile();
    QFile file(filePath);

    if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((doc->toPlainText()).toUtf8());
    file.close();

    if (fileUrl == m_fileUrl)
        return;

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

bool DocumentHandler::wasAlreadySaved(void){
    return !fileUrl().isEmpty();
}

void DocumentHandler::reset()
{
    emit boldChanged();
    emit italicChanged();
    emit underlineChanged();
    emit fontSizeChanged();
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

void DocumentHandler::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}
