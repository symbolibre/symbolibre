import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0


import DocumentHandler 1.0


ApplicationWindow {
    id: window
    visible: true
    width: 320
    height: 240
    title: document.fileName + qsTr(" - Symbolibre IDE")


    Menu {
        id: fileMenu

        MenuItem {
            text: qsTr("Ouvrir")
            onTriggered: openDialog.open()
        }

        MenuItem {
            text: qsTr("Enregister sous")
            onTriggered: saveDialog.open()
        }

        MenuItem {
            text: qsTr("Enregister")
            onTriggered: (document.wasAlreadySaved() ? document.saveAs(document.fileUrl) : saveDialog.open())
        }
    }

    Menu {
        id: languageMenu

        MenuItem {
            text: "Langage"
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
                text: "Fichier"
                onClicked: fileMenu.open()
            }

            ToolButton {
                id: runButton
                text: "Exécuter"
 /*
Should run the right interpreter/compiler, according to the language we are coding in, and show the result on screen, in an easy-to-manipulate window.
Should be able to return quickly to the ItemDelegate
*/
                //onClicked: interpreter.start()
            }

            ToolButton {
                id: configButton
                text: "Configuration"
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

    Rectangle {
        id: lineNumberBackground
        color: "light yellow"
        width: 32 //Should not be manually fixed
        height: parent.height
    }

    Flickable {
        id: flickable
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        anchors.fill: parent

        Column {
            id: lineNumber
            leftPadding: 0
            spacing: 0
            topPadding: 6 //Should not be manually handled ! Needs to be fixed later on
            Repeater {
                id: lineNumberRepeater
                model: textArea.lineCount

                Text {
                    text: index + 1 + " :"
                    font: textArea.font
                    horizontalAlignment: Text.AlignLeft
                    width: 25
                    leftPadding: 5
                    LayoutMirroring.enabled: true
                }
            }
        }

        TextArea.flickable: TextArea {
            id: textArea
            anchors.left: lineNumber.right
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
                id: textButton
                checked: true
                focus: true
                height: 30
                text: qsTr("Text")
                onClicked: document.setDocLanguage(1)
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
                onClicked: document.setDocLanguage(2)
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
                onClicked: document.setDocLanguage(3)
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
                onClicked: document.setDocLanguage(4)
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
                onClicked: document.setDocLanguage(5)
                onFocusChanged: checked = focus
                Keys.onEnterPressed: {
                        document.setDocLanguage(5)
                        document.startHighlighter()
                        popup.close()
                }
                KeyNavigation.down: textButton
            }

        }

    }

}
