import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

import org.symbolibre.applet 1.0
import org.symbolibre.catalog 1.0
import org.symbolibre.keyboard 1.0

SLStandardApplet {
    // Edition

    property alias document: editor.document
    property alias textArea: editor.textArea

    CatalogPopup {
        id: catalog
        width: parent.width - 1
        height: parent.height - 1
        anchors.centerIn: Overlay.overlay
        catalogId: "python"
        callback: insertSnippet
    }

    Editor {
        id: editor
        anchors.fill: parent
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

    FunctionBar.f1: FunctionKeyModel {
        text: qsTr("Catalog")
        onActivated: {
            catalog.setMenu("root");
            catalog.open();
        }
    }
    FunctionBar.f2: FunctionKeyModel {
        text: qsTr("Cycle\nfont size")
        onActivated: {
            var newSize = document.fontSize + 1;
            document.fontSize = (newSize < 14) ? newSize : 10;
        }
    }
    FunctionBar.f3: FunctionKeyModel {
        text: qsTr("Execute")
        onActivated: document.execute();
    }
    FunctionBar.f4: FunctionKeyModel {
        text: qsTr("Undo")
        onActivated: textArea.undo();
    }
    FunctionBar.f5: FunctionKeyModel {
        text: qsTr("Back to\nfiles")
        onActivated: popupStart.open();
    }

    function insertSnippet(key) {
        textArea.cursorPosition = document.insertSnippet(key);
    }

}
