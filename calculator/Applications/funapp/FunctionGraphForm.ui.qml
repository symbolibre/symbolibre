import QtQuick 2.4
import QtQuick.Layouts 1.12
import "../components"

FocusDistributor {
    id: root

    width: parent.width
    height: 220
    property alias mode: mode
    property alias stack: stack
    property alias menu: menu
    property alias graph: graph

    property alias xtext: xtext
    property alias ytext: ytext

    property ListModel functions

    SelectBar {
        id: mode
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 1
        clip: true
        text1: "Axis"
        text2: "Cursor"
        text3: "Zoom"
    }

    StackLayoutFocusDistributor {
        id: stack
        anchors.right: parent.right
        anchors.left: parent.left
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


            /*Connections {
                target: mainMenu
                onSavedIndexChanged: {
                    if (mainMenu.currentIndex == 1) {
                        graph.plot()
                    }
                }
            }*/
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
