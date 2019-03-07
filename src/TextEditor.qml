import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

import Qt.labs.platform 1.0

import DocumentHandler 1.0
import org.symbolibre.languagesModel 1.0

TextEditorForm {

    id: editor

    property string fileName: document.fileName

     LanguagesModel
     {
        id: langModel
     }

    // Document handling

    DocumentHandler {
        id: document
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        onLoaded: textArea.text = text
        Component.onCompleted:   { setDocLanguage(3)
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

    langselection.model: langModel
    langselection.onCurrentIndexChanged: document.setDocLanguage(langselection.currentIndex)

    // Dialogs

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        selectedNameFilter.index: document.docLanguage
        nameFilters: ["All files (*)", "Text files (*.txt)", "OCaml files (*ml)", "Python files (*.py)", "TI-Basic Files (*.tibs)", "Casio-Basic files (*.csbs)"]
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

}
