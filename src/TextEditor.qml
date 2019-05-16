import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

import DocumentHandler 1.0
import org.symbolibre.languagesModel 1.0
import org.symbolibre.snippetsModel 1.0

TextEditorForm {

    id: editor

    property string fileName: document.fileName
    property alias  document: document

     LanguagesModel
     {
        id: langModel
     }

     SnippetsModel
     {
         id: snippetModel
     }

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


    // Document handling

    DocumentHandler {
        id: document
        document: textArea.textDocument
        languageModel: langModel
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        onLoaded: textArea.text = text
    }


    // Slots

    openFileMenu.onTriggered: openDialog.open()
    saveAsMenu.onTriggered: saveDialog.open()
    saveMenu.onTriggered: (document.wasAlreadySaved() ? document.saveAs(document.fileUrl) : saveDialog.open())

    //Language Selection Popup

    Component {
        id: langDelegate
        Rectangle {
            id: langDelegateRect
            width: parent.width
            height: langDelegateText.height
            states: State {
                name: "selected"
                when: langDelegateRect.ListView.isCurrentItem
                PropertyChanges { target: langDelegateText; font.bold: true }
            }
            Text {
                id: langDelegateText
                text: model.languageName
            }
        }
    }

    langselection.model: langModel
    langselection.delegate: langDelegate

    // Snippet

    snippselection.model: snippetModel

    // File Explorer

    Component {
        id: fileDelegate
        Rectangle {
            id: fileDelegateRect
            width: parent.width
            height: fileDelegateText.height
            states: State {
                name: "selected"
                when: fileDelegateRect.ListView.isCurrentItem
                PropertyChanges { target: fileDelegateText; font.bold: true }
            }
            Text {
                id: fileDelegateText
                text: fileName
            }
        }
    }

    fileExplorerView.model: fileExplorerViewModel
    fileExplorerView.delegate: fileDelegate


    // Starting Popup

    Component {
        id: newOpenFileDelegate
        Rectangle {
            id: newOpenFileDelegateRect
            width: parent.width
            height: newOpenFileDelegateText.height
            states: State {
                name: "selected"
                when: newOpenFileDelegateRect.ListView.isCurrentItem
                PropertyChanges { target: newOpenFileDelegateText; font.bold: true }
            }
            Text {
                id: newOpenFileDelegateText
                text: model.choice
            }
        }
    }

    newOpenSelection.model: newOpenFileModel
    newOpenSelection.delegate: newOpenFileDelegate

    // Management of the popups

    Keys.onReturnPressed: {

        if (popup.activeFocus)
        {
            editor.document.setDocLanguage(langselection.currentIndex)
            editor.document.startHighlighter()
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
                editor.document.setDocLanguage(2)
                editor.document.startHighlighter()
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
        defaultSuffix: document.fileExtension
        //nameFilters: openDialog.nameFilters
        folder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
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
            snippetModel.snippets = document.snippets
            popupSnippets.open()
            snippselection.forceActiveFocus()
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

