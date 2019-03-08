import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import Qt.labs.platform 1.0

import Process 1.0

Item {
    property ApplicationWindow window: window

    property alias openFileMenu: openFileMenu
    property alias saveAsMenu: saveAsMenu
    property alias saveMenu: saveMenu
    property alias fileButton: fileButton
    property alias runButton: runButton
    property alias fileMenu: fileMenu
    property alias languageMenu: languageMenu
    property alias editorToolBar: editorToolBar
    property alias textArea: textArea
    property alias popup: popup
    property alias processExecute: processExecute
    property alias langselection: langselection

    Menu {
        id: fileMenu

        MenuItem {
            id: openFileMenu
            text: qsTr("Ouvrir")
        }

        MenuItem {
            id: saveAsMenu
            text: qsTr("Enregister sous")
        }

        MenuItem {
            id: saveMenu
            text: qsTr("Enregister")
        }
    }

    Menu {
        id: languageMenu

        MenuItem {
            text: "Langage"
        }
    }

    ToolBar {
        id: editorToolBar
        width: 320
        height: 29
        z: 1
        parent: window

        Row {
            id: fileRow

            ToolButton {
                id: fileButton
                text: "Fichier"
            }

            ToolButton {
                id: runButton
                text: "Exécuter"
            }
        }
    }

    Process {
        id: processExecute
    }

    Rectangle {
        x: 0
        y: 30
        visible: true
        width: 320
        height: window.height - 30

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
                spacing: 0
                y: 6 //Should not be manually handled - risk of bad alignement on other devices ! Needs to be fixed later on
                Repeater {
                    id: lineNumberRepeater
                    model: textArea.lineCount

                    Text {
                        text: index + 1 + ":"
                        font: textArea.font
                        horizontalAlignment: Text.AlignLeft
                        LayoutMirroring.enabled: true
                        width: lineNumberBackground.width
                    }
                }
            }

            TextArea.flickable: TextArea {
                id: textArea
                anchors.left: lineNumber.right
                anchors.top: editorToolBar.bottom
                rightPadding: 50 * window.width / 320 //Seems to be a known bug :
                //https://stackoverflow.com/questions/44471032/qml-textarea-strange-padding for more information
                textFormat: TextEdit.PlainText
                wrapMode: TextArea.Wrap
                selectByMouse: true
                persistentSelection: true
                background: null
                font.family: "Deja Vu Sans Mono"
                font.pointSize: 10
            }

            ScrollBar.vertical: ScrollBar {
            }
        }

        Popup {

            id: popup
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2

            parent: ApplicationWindow.contentItem

            modal: true
            focus: true
            visible: true

            ComboBox {
                id: langselection
                editable: false
                currentIndex: 1
                textRole: qsTr("languageName")
                Keys.forwardTo: editor
            }
        }
    }
}
