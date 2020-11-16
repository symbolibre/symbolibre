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

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import org.symbolibre.applet 0.2
import org.symbolibre.editor 0.2
import org.symbolibre.keyboard 0.2
import org.symbolibre.util 0.2

FocusScope {
    property alias document: document
    property alias textArea: textArea

    SourceDocument {
        id: document
        document: textArea.textDocument

        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd

        fontSize: 13
        onLoaded: {
            textArea.text = text
        }
    }

    /* Base sidebar on the left of the screen */

    Rectangle {
        id: sidebar
        color: "#e0e0e0"

        /* FIXME: This length 6 (width of a digit + spacing) should be
            FIXME: computed from font metrics */
        width: textArea.lineCount == 1 ? 18 : 12 + 6 *
            Math.ceil(Math.log(textArea.lineCount + 1) / Math.log(10))
        height: parent.height
    }

    /* Line numbers in sidebar */

    Flickable {
        id: sidebarFlickable
        contentHeight: textArea.height
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        /* The top margin is shared with the text area to keep line numbers
            and lines aligned. */
        anchors.fill: sidebar
        anchors.rightMargin: 3
        anchors.topMargin: textArea.topPadding

        Column {
            id: lineNumber
            spacing: 0

            Repeater {
                id: lineNumberRepeater
                model: textArea.lineCount

                Text {
                    text: index + 1

                    font.family: textArea.font.family
                    font.pixelSize: document.fontSize
                    font.hintingPreference: Font.PreferFullHinting

                    color: "#808080"
                    horizontalAlignment: Text.AlignRight
                    width: sidebar.width -
                            sidebarFlickable.anchors.rightMargin
                }
            }
        }

        /* This scrollbar is shared with the text area so that both
            elements scroll in sync. */
        ScrollBar.vertical: vertScrollBar
    }

    /* Text area */

    Flickable {
        id: textFlickable
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        anchors.right:  parent.right
        anchors.bottom: parent.bottom
        anchors.top:    parent.top
        anchors.left:   sidebar.right

        TextArea.flickable: TextArea {
            id: textArea
            topPadding: 2
            leftPadding: 2
            rightPadding: 0
            bottomPadding: 0
            focus: true

            textFormat: TextEdit.PlainText
            wrapMode: TextArea.NoWrap
            selectByMouse: true
            persistentSelection: true

            font.family: "Deja Vu Sans Mono"
            font.pixelSize: document.fontSize
            font.hintingPreference: Font.PreferFullHinting

            Keys.onPressed: {
                switch (event.key) {
                    case SLKey.If:
                        insertSnippet("if");
                        break;
                    case SLKey.Else:
                        insertSnippet("else");
                        break;
                    case SLKey.For:
                        insertSnippet("for");
                        break;
                    case SLKey.While:
                        insertSnippet("while");
                        break;
                    case SLKey.Input:
                        insertSnippet("input");
                        break;
                    case SLKey.Print:
                        insertSnippet("print");
                        break;
                    case SLKey.Enter:
                        insertAutoIndent();
                        break;

                    default:
                        return;
                }
                event.accepted = true;
            }
        }

        ScrollBar.vertical: vertScrollBar
        ScrollBar.horizontal: horizScrollBar
    }

    ScrollBar {
        id: vertScrollBar
        orientation: Qt.Vertical

        /* Anchor to the right of the text area */
        anchors.top:    parent.top
        anchors.bottom: parent.bottom
        anchors.right:  parent.right
    }

    ScrollBar {
        id: horizScrollBar
        orientation: Qt.Horizontal

        /* Anchor to the bottom of the text area */
        anchors.left:   sidebar.right
        anchors.right:  parent.right
        anchors.bottom: parent.bottom
    }

    /* Keyboard shortcuts */

    FunctionBar.f1: FunctionKeyModel {
        text: qsTr("Catalog")
        onActivated: {
            catalog.setMenu("root");
            catalog.open();
        }
    }
    FunctionBar.f2: FunctionKeyModel {
        text: qsTr("Save") + (document.modified ? "*" : "")
        onActivated: document.saveAs(document.filePath);
    }
    FunctionBar.f3: FunctionKeyModel {
        text: qsTr("Execute")  + (document.modified ? "*" : "")
        onActivated: {
            document.saveAs(document.filePath);
            document.execute();
        }
    }
    FunctionBar.f4: FunctionKeyModel {
        text: qsTr("Undo")
        onActivated: textArea.undo();
    }
}
