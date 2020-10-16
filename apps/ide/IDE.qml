import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

import org.symbolibre.applet 1.0
import org.symbolibre.catalog 1.0
import org.symbolibre.controls 1.0
import org.symbolibre.keyboard 1.0

SLStandardApplet {
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

    SLStackLayout {
        id: stackLayout
        currentIndex: 0
        anchors.fill: parent

        // Open an existing file
        FocusScope {
            focus: true

            ListView {
                id: fileExplorerView
                width: parent.width
                height: parent.height
                clip: true
                focus: true
                keyNavigationWraps: true
                model: fileExplorerViewModel

                delegate: ItemDelegate {
                    width: parent ? parent.width : 1
                    text: model.fileName
                    highlighted: ListView.isCurrentItem
                    onClicked: {
                        document.load(model.fileName);
                        stackLayout.currentIndex = 2;
                        stackLayout.children[2].forceActiveFocus();
                    }
                }
            }

            FolderListModel {
                id: fileExplorerViewModel
                showDirs: false // Would be buggy as we directly load the file when 'Return' is pressed
                folder: "file:///home/symbolibre/my_programs" // FIXME
            }

            FunctionBar.f5: FunctionKeyModel {
                text: qsTr("New file")
                onActivated: {
                    stackLayout.currentIndex = 1;
                    stackLayout.children[1].forceActiveFocus();
                }
            }
        }

        // Create a new file
        // This scope also allows the ColumnLayout to have margins
        FocusScope {
            ColumnLayout {
                anchors.fill: parent
                anchors.leftMargin: 8
                anchors.rightMargin: 8

                Label {
                    text: qsTr("Create a new file")
                    font.bold: true
                }
                RowLayout {
                    Label {
                        text: qsTr("Type")
                    }
                    ComboBox {
                        id: newFileLang

                        model: ["Python", "Plain text"]
                        focus: true
                        implicitHeight: 32
                        // Down is used to select options; better than nothing
                        KeyNavigation.right: newFileName
                    }
                }
                RowLayout {
                    width: parent.width
                    TextField {
                        id: newFileName
                        placeholderText: "file name"
                        Layout.fillWidth: true
                        implicitHeight: 30

                        KeyNavigation.right: newFileSubmit
                        KeyNavigation.down: newFileSubmit
                    }
                    Label { text: ".py" }
                }
                Button {
                    id: newFileSubmit
                    text: qsTr("Create")
                    Layout.alignment: Qt.AlignHCenter
                    implicitHeight: 30

                    onClicked: {
                        // TODO: Set language
                        // document.languageData = newFileLang.currentText;
                        stackLayout.currentIndex = 2;
                        stackLayout.children[2].forceActiveFocus();
                    }
                }

                FunctionBar.f5: FunctionKeyModel {
                    text: qsTr("Open file")
                    onActivated: {
                        stackLayout.currentIndex = 0;
                        stackLayout.children[0].forceActiveFocus();
                    }
                }
            }
        }

        // Editor
        Editor {
            id: editor

            FunctionBar.combine: true
            FunctionBar.f5: FunctionKeyModel {
                text: qsTr("Back to\nfiles")
                onActivated: {
                    stackLayout.currentIndex = 0;
                    stackLayout.children[0].forceActiveFocus();
                }
            }
        }
    }

    function insertSnippet(key) {
        textArea.cursorPosition = document.insertSnippet(key);
    }
}
