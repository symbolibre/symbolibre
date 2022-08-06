// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

// This file was initially based on the text editor example
// from Qt Quick Controls 2, available under the BSD license, reproduced below.

// Copyright 2017 The Qt Company Ltd. <https://www.qt.io/licensing/>
// SPDX-License-Identifier: BSD-3-Clause

/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Redistribution and use in source and binary forms, with or without
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
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#ifndef SOURCEDOCUMENT_HPP
#define SOURCEDOCUMENT_HPP

#include <QFont>
#include <QObject>
#include <QTextCursor>
#include <QTextDocument>
#include <QQuickTextDocument>
#include <QPlainTextEdit>

#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Theme>

#include <qtermwidget5/qtermwidget.h>

#include "languagesmodel.hpp"

/** SourceDocument is a wrapper around a text document in the source code
    editor. It binds code to language definitions and performs syntax
    highlighting, manages the file's storage path (during Open, Save as, etc),
    and exposes graphical parameters such as the font size. It is used
    alongside a text edition area to provide code edition functionalities. It
    can be seen as an extension of TextDocument.

    This is, in essence, a QML object. Several properties must be QML-bound for
    advanced editing to work, and the interface is conceived for efficient use
    in a QML program.

    When using SourceDocument, instantiate it along a QML editor such as
    TextEdit or TextArea. Then, bind its document to the textDocument of the
    editor, and its cursorPosition, selectionStart and selectionEnd properties
    to that of the editor. This allows SourceDocument to perform smart snippet
    insertion, among other features. For example:

        SourceDocument {
            id: srcdoc
            document: editor.textDocument
            cursorPosition: editor.cursorPosition
            selectionStart: editor.selectionStart
            selectionEnd: editor.selectionEnd
        }
    **/
class SourceDocument: public QObject
{
    Q_OBJECT

    /** Underlying Qt Quick document. This attribute is used only in QML code
        to bind SourceDocument to a text area. **/
    Q_PROPERTY(QQuickTextDocument *document
        MEMBER m_document NOTIFY documentChanged)

    /** Whether the document has been modified. **/
    Q_PROPERTY(bool modified
        READ modified WRITE setModified NOTIFY modifiedChanged)

    /** Cursor position in the edition widget. Used for snippet insertion;
        should be bound to the cursorPosition attribute of the text area. **/
    Q_PROPERTY(int cursorPosition
        MEMBER m_cursorPosition NOTIFY cursorPositionChanged)

    /** Start of selection. Bind it to the selectionStart of the text area. **/
    Q_PROPERTY(int selectionStart
        MEMBER m_selectionStart NOTIFY selectionStartChanged)

    /** End of selection. Bind it to the selectionEnd of the text area. **/
    Q_PROPERTY(int selectionEnd
        MEMBER m_selectionEnd NOTIFY selectionEndChanged)

    /** Editor font size (in pixels), can be changed dynamically. **/
    Q_PROPERTY(int fontSize
        MEMBER m_fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    /** Storage path of the file being edited. Initially, this is empty for new
        files and the source path for files that have been opened directly.
        Changes when the file is saved under a new name. **/
    Q_PROPERTY(QString filePath
        READ filePath NOTIFY filePathChanged)

    /** Name, canonical extension and interpreter of the currently selected
        language. **/
    Q_PROPERTY(LanguageData *languageData
        READ languageData NOTIFY languageDataChanged)

    /** Supported set of languages, with their canonical extensions and
        interpreter commands. **/
    Q_PROPERTY(LanguagesModel *languages
        READ languages NOTIFY languagesChanged)

public:
    explicit SourceDocument(QWidget *parent = nullptr);

    /* QML accessors */
    bool modified() const;
    LanguageData *languageData() const;
    LanguagesModel *languages();
    QString filePath() const;

    /* QML mutators */
    void setModified(bool modified);
    void setFontSize(int size);


    QString syntaxDefinitionName(void) const;

    /** Insert a snippet at the current cursor position. **/
    Q_INVOKABLE int insertSnippet(QString snippet);
    /** Insert an auto-indented newline at the current cursor position. **/
    Q_INVOKABLE int insertAutoIndent();

public Q_SLOTS:
    /** Load a file into the editor. The previous file is discarded
        regardless of whether is had unsaved contents. The syntax highlighter
        is initialized from the file's extension. **/
    void load(const QString &filePath, bool create=false);

    /** Create a new file and load it into the editor. **/
    void create(const QString &filePath);

    /** Saves the contents of the editor to a file. This function is suitable
        for both Save and Save As. Always writes as UTF-8. **/
    void saveAs(const QString &filePath);

    void execute();

Q_SIGNALS:
    /* QML property update signals */
    void documentChanged();
    void modifiedChanged();
    void languageDataChanged(LanguageData *lang);
    void languagesChanged(LanguagesModel *languages);
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();

    void fontSizeChanged();
    void filePathChanged();

    /** A new file has been loaded into the editor. **/
    void loaded(const QString &text);
    /** A generic error has occurred, typically a file cannot be opened or
        written to. **/
    void error(const QString &message);


private:
    QTextCursor textCursor() const;
    QTextDocument *textDocument() const;

    QTermWidget *m_term;
    QTemporaryFile *m_termInput;
    bool m_executionFinished;

    /* Properties inherited from the text area */
    QQuickTextDocument *m_document;
    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;

    /* Font size, in pixels */
    int m_fontSize;
    /* Path to the currently open file (may be empty) */
    QString m_filePath;

    /* Current language and language model */
    LanguagesModel m_languages;
    LanguageData *m_languageData;

    /* Highlighting context */
    KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;
    KSyntaxHighlighting::Repository m_repository;
};

#endif /* SOURCEDOCUMENT_HPP */
