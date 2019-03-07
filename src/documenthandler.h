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

#ifndef DOCUMENTHANDLER_H
#define DOCUMENTHANDLER_H

#include <QFont>
#include <QObject>
#include <QTextCursor>
#include <QUrl>
#include <QPlainTextEdit>
#include <syntaxhighlighter.h>
#include<repository.h>
#include<definition.h>
#include<theme.h>

#include <languagesmodel.h>

QT_BEGIN_NAMESPACE
class QTextDocument;
class QQuickTextDocument;
QT_END_NAMESPACE

class DocumentHandler : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)

    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(bool underline READ underline WRITE setUnderline NOTIFY underlineChanged)

    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileExtension READ fileExtension NOTIFY fileUrlChanged NOTIFY docLanguageChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl NOTIFY fileUrlChanged)

    Q_PROPERTY(int docLanguage READ docLanguage WRITE setDocLanguage NOTIFY docLanguageChanged)

    Q_PROPERTY(LanguagesModel *languageModel READ languageModel WRITE setLanguageModel NOTIFY languageModelChanged)

public:
    explicit DocumentHandler(QWidget *parent = nullptr);

    QQuickTextDocument *document() const;
    void setDocument(QQuickTextDocument *document);

    int cursorPosition() const;
    void setCursorPosition(int position);

    int selectionStart() const;
    void setSelectionStart(int position);

    int selectionEnd() const;
    void setSelectionEnd(int position);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    bool bold() const;
    void setBold(bool bold);

    bool italic() const;
    void setItalic(bool italic);

    bool underline() const;
    void setUnderline(bool underline);

    int fontSize() const;
    void setFontSize(int size);

    QString fileName() const;
    QString fileExtension() const;
    QUrl fileUrl() const;

    int docLanguage() const;

    QString syntaxDefinitionName(void) const;

    int startLine(void);

    LanguagesModel *languageModel() const;
    void setLanguageModel(LanguagesModel *langModel);


public Q_SLOTS:
    void load(const QUrl &fileUrl);
    void saveAs(const QUrl &fileUrl);
    void setDocLanguage(int language);
    void setDocLanguageFromExtension(QString fileExt);
    bool wasAlreadySaved(void);
    void startHighlighter(void);

Q_SIGNALS:
    void documentChanged();

    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();

    void boldChanged();
    void italicChanged();
    void underlineChanged();

    void fontSizeChanged();

    void textChanged();
    void fileUrlChanged();

    void docLanguageChanged();

    void loaded(const QString &text);
    void error(const QString &message);

    void languageModelChanged();

private:
    void reset();
    QTextCursor textCursor() const;
    QTextDocument *textDocument() const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    int sidebarWidth() const;
    void sidebarPaintEvent(QPaintEvent *event);
    void updateSidebarGeometry();
    void updateSidebarArea(const QRect& rect, int dy);

    QQuickTextDocument *m_document;

    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;
    int m_fontSize;
    QUrl m_fileUrl;
    int m_docLanguage;

    LanguagesModel *m_languageModel;

    KSyntaxHighlighting::SyntaxHighlighter *m_highlighter;
    KSyntaxHighlighting::Repository m_repository;

};

#endif // DOCUMENTHANDLER_H
