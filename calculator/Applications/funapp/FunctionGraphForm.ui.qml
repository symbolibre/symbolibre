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

        GraphInterface {
            id: graph
            mode_int: mode.currentIndex
            functions: root.functions

            anchors.fill: parent
            anchors.bottomMargin: 15
            anchors.topMargin: 0
            anchors.leftMargin: 0
            anchors.rightMargin: 0
            clip: true
        }
    }

    Text {
        id: xtext
        anchors.bottom: parent.bottom
        x: 160
        y: 200
        width: 75
        height: 20
        anchors.bottomMargin: 0
    }

    Text {
        id: ytext
        anchors.bottom: parent.bottom
        x: 252
        y: 200
        width: 68
        height: 20
        anchors.bottomMargin: 0
    }
}
