import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0

import SymbolibreTextEditor 1.0


ApplicationWindow {
    id: window
    visible: true
    width: 320
    height: 240
    title: document.fileName + qsTr(" - Symbolibre IDE")


    MenuBar {
        Menu {

            title: qsTr("File")

            MenuItem {
                text: qsTr("Open")
                onTriggered: openDialog.open()
            }

            MenuItem {
                text: qsTr("Save")
                onTriggered: saveDialog.open()
            }
        }
    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: 1
        nameFilters: ["All files (*)", "Text files (*.txt)", "OCaml files (*ml)", "Python files (*.py)", "TI-Basic Files (*.bsti)", "Casio-Basic files (*.bscs)"]
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.load(file)
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.fileType
        nameFilters: openDialog.nameFilters
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.saveAs(file)
    }


    header: ToolBar {

        Row {
            id: fileRow

            ToolButton {
                id: typeButton
                text: "File type"
                onClicked: popup.open()
            }

            ToolButton {
                id: openButton
                text: "Open"
                onClicked: openDialog.open()
            }
            ToolButton {
                id: saveAsButton
                text: "Save as ..."
                onClicked: saveDialog.open()
            }
            ToolButton {
                id: saveButton
                text: "Save"
                onClicked: document.saveAs(document.fileUrl)
            }
        }

    }

    DocumentHandler {
        id: document
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd

        onLoaded: {
            textArea.text = text
        }
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        anchors.fill: parent

        TextArea.flickable: TextArea {
            id: textArea
            textFormat: Qt.RichText
            wrapMode: TextArea.Wrap
            focus: true
            selectByMouse: true
            persistentSelection: true
            // Different styles have different padding and background
            // decorations, but since this editor is almost taking up the
            // entire window, we don't need them.
            background: null

            onLinkActivated: Qt.openUrlExternally(link)
        }

        ScrollBar.vertical: ScrollBar {}
    }

    Popup {
        id: popup
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        parent: ApplicationWindow.contentItem

        modal: true
        focus: true

        Column {

            RadioButton {
                checked: true
                height: 30
                text: qsTr("Text file")
            }

            RadioButton {
                height: 30
                text: qsTr("Ocaml file")
            }

            RadioButton {
                height: 30
                text: qsTr("Python file")
            }

            RadioButton {
                height: 30
                text: qsTr("TI-Basic file")
            }

            RadioButton {
                height: 30
                text: qsTr("Casio-Basic file")
            }

            Button {
                height: 20
                id: validateType
                text: qsTr("OK")
                onClicked: popup.close()
            }
        }

    }

}
