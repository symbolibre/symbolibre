import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import org.symbolibre.editor 1.0
import org.symbolibre.keyboard 1.0

Item {
    property alias document: document
    property alias textArea: textArea

    SourceEditor {
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
}