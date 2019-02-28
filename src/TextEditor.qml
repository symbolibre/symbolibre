import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import Qt.labs.platform 1.0

import DocumentHandler 1.0

TextEditorForm {
    id: editor

    property int currLanguage: 1
    property string fileName: document.fileName

    // Document handling

    DocumentHandler {
        id: document
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        onLoaded: textArea.text = text
        Component.onCompleted:   { //setDocLanguage(1)
            document.startHighlighter()
        }
    }

    // Slots

    openFileMenu.onTriggered: openDialog.open()
    saveAsMenu.onTriggered: saveDialog.open()
    saveMenu.onTriggered: (document.wasAlreadySaved() ? document.saveAs(document.fileUrl) : saveDialog.open())
    fileButton.onClicked: fileMenu.open()

    // Used to execute the code, will be impleted later on
    //runButton.onClicked: interpreter.start()

    configButton.onClicked: configdialog.open()

    // -- Popup language choice

    textButton.onClicked:
    {
        document.setDocLanguage(1)

    }
    textButton.onFocusChanged: {
        textButton.checked = textButton.focus
        document.setDocLanguage(1)
    }

    ocamlButton.onClicked: document.setDocLanguage(2)
    ocamlButton.onFocusChanged:
    {
        ocamlButton.checked = ocamlButton.focus
        document.setDocLanguage(2)
    }

    pythonButton.onClicked: document.setDocLanguage(3)
    pythonButton.onFocusChanged:
    {

        pythonButton.checked = pythonButton.focus
        document.setDocLanguage(3)
    }


    tiBasicButton.onClicked: document.setDocLanguage(4)
    tiBasicButton.onFocusChanged:
    {
        tiBasicButton.checked = tiBasicButton.focus
        document.setDocLanguage(4)
    }


    casioBasicButton.onClicked: document.setDocLanguage(5)
    casioBasicButton.onFocusChanged:
    {
        casioBasicButton.checked = casioBasicButton.focus
        document.setDocLanguage(5)
    }

    Keys.onPressed: {
        document.startHighlighter()
        popup.close()
    }


    // Dialogs

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

    ConfigurationForm {
        id: configdialog
    }





}
