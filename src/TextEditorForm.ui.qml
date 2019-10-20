import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Window 2.2
import Qt.labs.platform 1.0
import Qt.labs.folderlistmodel 2.2

Item {
    property ApplicationWindow window: window

    /* Expose most ID'd objects as properties */
    property alias openFileMenu: openFileMenu
    property alias saveAsMenu: saveAsMenu
    property alias saveMenu: saveMenu
    property alias fileMenu: fileMenu
    property alias languageMenu: languageMenu
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

    Rectangle {
        width: window.width
        height: window.height

        /* Base sidebar on the left of the screen */

        Rectangle {
            id: sidebar
            color: "#e0e0e0"

            /* FIXME: This length 6 (width of a digit + spacing) should be
               FIXME: computed from font metrics */
            width: textArea.lineCount == 1 ? 18 : 12 + 6 *
                Math.ceil(Math.log(textArea.lineCount + 1) / Math.log(10))
            height: parent.height
        }

        /* Line numbers in sidebar */

        Flickable {
            id: sidebarFlickable
            contentHeight: textArea.height
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            /* The top margin is shared with the text area to keep line numbers
               and lines aligned. */
            anchors.fill: sidebar
            anchors.rightMargin: 3
            anchors.topMargin: textArea.topPadding

            Column {
                id: lineNumber
                spacing: 0

                Repeater {
                    id: lineNumberRepeater
                    model: textArea.lineCount

                    Text {
                        text: index + 1

                        font.family: textArea.font.family
                        font.pixelSize: document.fontSize
                        font.hintingPreference: Font.PreferFullHinting

                        color: "#808080"
                        horizontalAlignment: Text.AlignRight
                        width: sidebar.width -
                               sidebarFlickable.anchors.rightMargin
                    }
                }
            }

            /* This scrollbar is shared with the text area so that both
               elements scroll in sync. */
            ScrollBar.vertical: vertScrollBar
        }

        /* Text area */

        Flickable {
            id: textFlickable
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            anchors.right:  parent.right
            anchors.bottom: parent.bottom
            anchors.top:    parent.top
            anchors.left:   sidebar.right

            TextArea.flickable: TextArea {
                id: textArea
                topPadding: 2
                leftPadding: 2
                rightPadding: 0
                bottomPadding: 0

                textFormat: TextEdit.PlainText
                wrapMode: TextArea.NoWrap
                selectByMouse: true
                persistentSelection: true

                font.family: "Deja Vu Sans Mono"
                font.pixelSize: document.fontSize
                font.hintingPreference: Font.PreferFullHinting
            }

            ScrollBar.vertical: vertScrollBar
            ScrollBar.horizontal: horizScrollBar
        }

        ScrollBar {
            id: vertScrollBar
            orientation: Qt.Vertical

            /* Anchor to the right of the text area */
            anchors.top:    parent.top
            anchors.bottom: parent.bottom
            anchors.right:  parent.right
        }

        ScrollBar {
            id: horizScrollBar
            orientation: Qt.Horizontal

            /* Anchor to the bottom of the text area */
            anchors.left:   sidebar.right
            anchors.right:  parent.right
            anchors.bottom: parent.bottom
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
                width: parent.width
                height: parent.height
                clip: true
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
                width: parent.width
                height: parent.height
                clip: true
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
                clip: true
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
