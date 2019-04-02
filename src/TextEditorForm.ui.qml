import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

Item {
    property ApplicationWindow window: window

    property alias openFileMenu: openFileMenu
    property alias saveAsMenu: saveAsMenu
    property alias saveMenu: saveMenu
    //property alias fileButton: fileButton
    //property alias runButton: runButton
    property alias fileMenu: fileMenu
    property alias languageMenu: languageMenu
    //property alias editorToolBar: editorToolBar
    property alias textArea: textArea
    property alias popup: popup
    property alias popupSnippets: popupSnippets
    property alias langselection: langselection
    property alias snippselection: snippselection
    property alias popupFileExplorer: popupFileExplorer
    property alias fileExplorerView: fileExplorerView
    property alias popupStart: popupStart
    property alias newOpenSelection: newOpenSelection

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

    /*
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
                spacing: -3
            }
        }
    }
*/
    Rectangle {
        visible: true
        width: window.width
        height: window.height

        Rectangle {
            id: lineNumberBackground
            color: "light yellow"
            width: textArea.lineCount == 1 ? 26 : 20 + 6 * Math.ceil(
                                                 Math.log(
                                                     textArea.lineCount + 1) / Math.log(
                                                     10))
            height: parent.height
        }

        Flickable {
            id: lineNumberFlickable
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            anchors.fill: lineNumberBackground
            contentHeight: textArea.height

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

            ScrollBar.vertical: vertScrollBar
        }

        Flickable {
            id: textFlickable
            flickableDirection: Flickable.HorizontalAndVerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.left: lineNumberBackground.right

            TextArea.flickable: TextArea {
                id: textArea
                anchors.left: parent.left
                anchors.top: parent.top
                rightPadding: 20 * window.width / 320 //Seems to be a known bug :
                //https://stackoverflow.com/questions/44471032/qml-textarea-strange-padding for more information
                textFormat: TextEdit.PlainText
                wrapMode: TextArea.NoWrap
                selectByMouse: true
                persistentSelection: true
                background: null
                font.family: "Deja Vu Sans Mono"
                font.pixelSize: 13
            }

            ScrollBar.vertical: lineNumberFlickable.ScrollBar.vertical
            ScrollBar.horizontal: horizScrollBar
        }

        ScrollBar {
            id: vertScrollBar
            orientation: Qt.Vertical
        }

        ScrollBar {
            id: horizScrollBar
            orientation: Qt.Horizontal
        }

        Popup {

            id: popup
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 180
            height: 100

            parent: ApplicationWindow.contentItem

            modal: true
            focus: true
            visible: false

            ListView {
                id: langselection
                height: parent.height
                keyNavigationWraps: true
                Keys.forwardTo: editor
            }
        }

        Popup {

            id: popupFileExplorer
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 200
            height: 200

            parent: ApplicationWindow.contentItem

            modal: true
            focus: true
            visible: false

            ListView {
                id: fileExplorerView
                height: parent.height
                keyNavigationWraps: true
                Keys.forwardTo: editor
            }
        }

        Popup {

            id: popupSnippets
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 180

            parent: ApplicationWindow.contentItem

            modal: true
            focus: true
            visible: false

            ComboBox {
                id: snippselection
                width: parent.width
                editable: false
                currentIndex: 1
                textRole: qsTr("snippetKey")
                Keys.forwardTo: editor
            }
        }

        Popup {
            id: popupStart
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 180
            height: 60

            parent: ApplicationWindow.contentItem

            modal: true
            focus: true
            visible: true

            ListView {
                id: newOpenSelection
                width: parent.width
                height: parent.height
                keyNavigationWraps: true
                Keys.forwardTo: editor

            }
        }
    }
}
/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/

