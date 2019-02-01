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


    Menu {
        id: menu
        title: qsTr("File")

        MenuItem {
            text: qsTr("Open")
            onTriggered: openDialog.open()
        }

        MenuItem {
            text: qsTr("Save as ...")
            onTriggered: saveDialog.open()
        }

        MenuItem {
            text: qsTr("Save")
            onTriggered: (document.wasAlreadySaved() ? document.saveAs(document.fileUrl) : saveDialog.open())
        }
    }

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: document.docLanguage
        nameFilters: ["All files (*)", "Text files (*.txt)", "OCaml files (*ml)", "Python files (*.py)", "TI-Basic Files (*.bsti)", "Casio-Basic files (*.bscs)"]
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.load(file)
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.fileType //Might need to use docLanguage !
        nameFilters: openDialog.nameFilters
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.saveAs(file)
    }


    header: ToolBar {

        Row {
            id: fileRow

            ToolButton {
                id: fileButton
                text: "File"
                onClicked: menu.open()
            }

            ToolButton {
                id: runButton
                text: "Run"
 /*
Should run the right interpreter/compiler, according to the language we are coding in, and show the result on screen, in an easy-to-manipulate window.
Should be able to return quickly to the ItemDelegate
*/
                //onClicked: interpreter.start()
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
        boundsBehavior: Flickable.StopAtBounds
        anchors.fill: parent

        TextArea.flickable: TextArea {
            id: textArea
            textFormat: TextEdit.PlainText
            wrapMode: TextArea.Wrap
            focus: true
            selectByMouse: true
            persistentSelection: true
            // Different styles have different padding and background
            // decorations, but since this editor is almost taking up the
            // entire window, we don't need them.
            background: null
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
        visible: true

        Column {

            RadioButton {
                checked: true
                height: 30
                text: qsTr("Text file")
                onClicked: document.setDocLanguage(1)
            }

            RadioButton {
                height: 30
                text: qsTr("Ocaml file")
                onClicked: document.setDocLanguage(2)
            }

            RadioButton {
                height: 30
                text: qsTr("Python file")
                onClicked: document.setDocLanguage(3)
            }

            RadioButton {
                height: 30
                text: qsTr("TI-Basic file")
                onClicked: document.setDocLanguage(4)
            }

            RadioButton {
                height: 30
                text: qsTr("Casio-Basic file")
                onClicked: document.setDocLanguage(5)
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
