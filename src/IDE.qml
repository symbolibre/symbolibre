import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

import org.symbolibre.editor 1.0
import org.symbolibre.keyboard 1.0

Item {
    // Edition

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

    Popup {
        id: langPopup
        width: 180
        height: 100
        anchors.centerIn: Overlay.overlay

        modal: true
        focus: true

        ListView {
            id: langselection
            width: parent.width
            height: parent.height
            focus: true
            clip: true
            keyNavigationWraps: true

            model: ["Python", "Plain text"]
            delegate: ItemDelegate {
                text: modelData
                width: parent.width
                highlighted: ListView.isCurrentItem
            }

            Keys.onReturnPressed: {
                editor.document.setLanguage(langselection.currentText) // TODO
                langPopup.close()
                textArea.forceActiveFocus()

            }
        }
    }

    Popup {

        id: popupFileExplorer
        anchors.centerIn: Overlay.overlay
        width: 200
        height: 200

        modal: true
        focus: true

        ListView {
            id: fileExplorerView
            width: parent.width
            height: parent.height
            focus: true
            clip: true
            keyNavigationWraps: true
            model: fileExplorerViewModel
            delegate: ItemDelegate {
                width: parent.width
                text: model.fileName
                highlighted: ListView.isCurrentItem
                onClicked: {
                    document.load(model.fileName);
                    popupFileExplorer.close();
                }
            }
        }

        FolderListModel {
            id: fileExplorerViewModel
            showDirs: false // Would be buggy as we directly load the file when 'Return' is pressed
            folder: "file:///home/symbolibre/my_programs" // FIXME
        }
    }

    Popup {
        id: popupSnippets
        anchors.centerIn: Overlay.overlay
        width: 180

        modal: true
        focus: true

        ComboBox {
            id: snippselection
            width: parent.width
            focus: true
            editable: false
            model: SnippetsModel { snippets: document.snippets }
            currentIndex: 1
            textRole: qsTr("snippetKey")
            Keys.onReturnPressed: {
                textArea.cursorPosition = document.insertSnippet(currentText);
                popupSnippets.close();
            }
        }
    }

    Popup {
        id: popupStart
        anchors.centerIn: Overlay.overlay

        modal: true
        focus: true
        visible: true

        ColumnLayout {
            id: newOpenLayout
            Button {
                id: newButton
                Layout.alignment: Qt.AlignHCenter
                focus: true
                text: qsTr("Create new file")
                KeyNavigation.up: openButton
                KeyNavigation.down: openButton
                onClicked: { popupStart.close(); langPopup.open() }
            }
            Button {
                id: openButton
                Layout.alignment: Qt.AlignHCenter
                focus: true
                text: qsTr("Edit existing file")
                onClicked: { popupStart.close(); popupFileExplorer.open() }
            }
        }
    }

    // Keyboard shortcuts

    Shortcut {
        sequence: "F3"
        onActivated: {
            document.execute()
        }
    }

    Shortcut {
        sequence: "Shift+F4"
        onActivated: popupStart.open()
    }

    Shortcut {
        sequence: "F1"
        onActivated: {
            popupSnippets.open()
        }
    }

    Shortcut {
        sequence: "F2"
        onActivated: {
            document.fontSize -= 1
        }
    }
    Shortcut {
        sequence: "Shift+F2"
        onActivated: {
            document.fontSize += 1
        }
    }

    Shortcut {
        sequence: "Shift+Backspace"
        onActivated: textArea.undo()
    }

    function insertSnippet(key) {
        textArea.cursorPosition = document.insertSnippet(key);
    }

}
