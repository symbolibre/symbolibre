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
        defaultSuffix: document.fileExtension
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
        onLoaded: textArea.text = text
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
                focus: true
                height: 30
                text: qsTr("Text")
                //onClicked: document.setDocLanguage(1)
                onFocusChanged: checked = focus
                Keys.onEnterPressed: {
                    document.setDocLanguage(1)
                    document.startHighlighter()
                    popup.close()
                }
                KeyNavigation.down: ocamlButton

            }

            RadioButton {
                id: ocamlButton
                height: 30
                text: qsTr("OCaml")
                //onClicked: document.setDocLanguage(2)
                onFocusChanged: checked = focus
                Keys.onEnterPressed: {
                    document.setDocLanguage(2)
                    document.startHighlighter()
                    popup.close()
                }
                KeyNavigation.down: pythonButton
            }

            RadioButton {
                id: pythonButton
                height: 30
                text: qsTr("Python")
                //onClicked: document.setDocLanguage(3)
                onFocusChanged: checked = focus
                Keys.onEnterPressed: {
                    document.setDocLanguage(3)
                    document.startHighlighter()
                    popup.close()
                }
                KeyNavigation.down: tiBasicButton
            }

            RadioButton {
                id: tiBasicButton
                height: 30
                text: qsTr("TI Basic")
                //onClicked: document.setDocLanguage(4)
                onFocusChanged: checked = focus
                Keys.onEnterPressed: {
                    document.setDocLanguage(4)
                    document.startHighlighter()
                    popup.close()
                }
                KeyNavigation.down: casioBasicButton

            }

            RadioButton {
                id: casioBasicButton
                height: 30
                text: qsTr("Casio Basic")
                //onClicked: document.setDocLanguage(5)
                onFocusChanged: checked = focus
                Keys.onEnterPressed: {
                        document.setDocLanguage(5)
                        document.startHighlighter()
                        popup.close()
                }
            }

            /*
            Button {
                height: 20
                id: validateLanguage
                text: qsTr("OK")
                onClicked: {
                    document.startHighlighter()
                    popup.close()
                }
            }
            */
        }

    }

}
