import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

import SourceEditor 1.0
import org.symbolibre.snippetsModel 1.0

TextEditorForm {
    id: editor

    property string filePath: document.filePath
    property alias  document: document

    snippselection.model: SnippetsModel { snippets: document.snippets }

     FolderListModel {
         id: fileExplorerViewModel
         showDirs: false // Would be buggy as we directly load the file when 'Return' is pressed
         folder: "file:///home/symbolibre/my_programs" // FIXME
     }

     ListModel {
         id: newOpenFileModel
         ListElement {
             choice: "New file"
         }
         ListElement {
             choice: "Open file"
         }
     }


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


    // Slots

    openFileMenu.onTriggered: openDialog.open()
    saveAsMenu.onTriggered: saveDialog.open()
    saveMenu.onTriggered: (document.wasAlreadySaved() ? document.saveAs(document.fileUrl) : saveDialog.open())

    //Language Selection Popup


    langselection.model: ["Python", "Plain text"]
    langselection.delegate: ItemDelegate {
        width: parent.width
        text: modelData
        highlighted: ListView.isCurrentItem
    }

    // File Explorer

    fileExplorerView.model: fileExplorerViewModel
    fileExplorerView.delegate: ItemDelegate {
        width: parent.width
        text: model.fileName
        highlighted: ListView.isCurrentItem
    }


    // Starting Popup

    newOpenSelection.model: newOpenFileModel
    newOpenSelection.delegate: ItemDelegate {
        width: parent.width
        text: model.choice
        highlighted: ListView.isCurrentItem
    }

    // Management of the popups

    Keys.onReturnPressed: {

        if (popup.activeFocus)
        {
            editor.document.setLanguage(langselection.currentText) // TODO
            popup.close()
            textArea.forceActiveFocus()
        }

        if (popupSnippets.activeFocus)
        {
            textArea.cursorPosition = document.insertSnippet(snippselection.currentText)
            popupSnippets.close()
            textArea.forceActiveFocus()
        }

        if (popupFileExplorer.activeFocus)
        {
            document.load(fileExplorerView.model.get(fileExplorerView.currentIndex, "filePath"))
            popupFileExplorer.close()
            textArea.forceActiveFocus()
        }

        if (popupStart.activeFocus){
            if (newOpenSelection.currentIndex == 0){
                popupStart.close()
                popup.open()
                langselection.forceActiveFocus()
            }
            else if (newOpenSelection.currentIndex == 1){
                popupStart.close()
                popupFileExplorer.open()
                fileExplorerView.forceActiveFocus()
            }
        }
    }

    // Dialogs

    FileDialog {
        id: openDialog
        fileMode: FileDialog.OpenFile
        //selectedNameFilter.index: document.docLanguage
        //nameFilters: ["All files (*)", "Text files (*.txt)", "OCaml files (*ml)", "Python files (*.py)", "TI-Basic Files (*.tibs)", "Casio-Basic files (*.csbs)"]
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
        onAccepted: document.load(file)
    }

    FileDialog {
        id: saveDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: document.languageData.extension
        //nameFilters: openDialog.nameFilters
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + document.filePathDefault
        onAccepted: document.saveAs(file)
    }


    // Binding for the keyboard

    Shortcut {
        sequence: "Ctrl+o"
        onActivated: openDialog.open()
    }

    Shortcut {
        sequence: "Ctrl+s"
        onActivated: saveDialog.open()
    }

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
            snippselection.forceActiveFocus()
        }
    }

    Shortcut {
        sequence: "F2"
        onActivated: {
            document.fontSize = document.fontSize - 1
        }
    }
    Shortcut {
        sequence: "Shift+F2"
        onActivated: {
            document.fontSize = document.fontSize + 1
        }
    }

    Shortcut {
        sequence: "F5"
        onActivated: textArea.cursorPosition = document.insertSnippet("if")
    }
    Shortcut {
        sequence: "Shift+F5"
        onActivated: textArea.cursorPosition = document.insertSnippet("else")
    }

    Shortcut {
        sequence: "F6"
        onActivated: textArea.cursorPosition = document.insertSnippet("for")
    }
    Shortcut {
        sequence: "Shift+F6"
        onActivated: textArea.cursorPosition = document.insertSnippet("while")
    }

    Shortcut {
        sequence: "F7"
        onActivated: textArea.cursorPosition = document.insertSnippet("print")
    }
    Shortcut {
        sequence: "Shift+F7"
        onActivated: textArea.cursorPosition = document.insertSnippet("input")
    }

    Shortcut {
        sequence: "Shift+Backspace"
        onActivated: textArea.undo()
    }

}
