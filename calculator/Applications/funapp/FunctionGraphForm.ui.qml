import QtQuick 2.4
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.2
import "../components"

FocusDistributor {
    id: root

    height: 220
    property alias mode: mode
    property alias stack: stack
    property alias menu: menu
    property alias graph: graph

    property alias xtext: xtext
    property alias ytext: ytext

    property ListModel functions

    SLTabBar {
        id: mode
        focus: true
        width: parent.width
        height: 20
        anchors.top: parent.top
        clip: true
        Repeater {
            model: ["Axis", "Cursor", "Zoom"]
            TabButton {
                text: modelData
            }
        }
    }

    SLStackLayout {
        id: stack
        width: parent.width
        anchors.bottom: parent.bottom
        anchors.top: mode.bottom
        anchors.margins: 0
        currentIndex: Math.min(mode.currentIndex, 1)

        FunctionMenu {
            id: menu
        }

        FocusScope {
            focus: true

            GraphInterface {
                id: graph
                focus: true
                width: parent.width
                height: parent.height - 15
                mode_int: mode.currentIndex
                functions: root.functions
                clip: true
            }

            Label {
                id: xtext
                anchors.bottom: parent.bottom
                anchors.right: ytext.left
                width: 75
                height: 20
                anchors.bottomMargin: 0
            }

            Label {
                id: ytext
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: 75
                height: 20
                anchors.bottomMargin: 0
            }
        }
    }

}
