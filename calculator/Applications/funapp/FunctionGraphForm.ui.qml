import QtQuick 2.4
import "../components"

FocusDistributor {
    width: 320
    height: 220
    property alias mode: mode
    property alias stack: stack
    property alias menu: menu
    property alias graph: graph

    property alias xtext: xtext
    property alias ytext: ytext

    SelectBar {
        id: mode
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        clip: true
        anchors.top: parent.top
        anchors.topMargin: 1
        text1: "Axis"
        text2: "Cursor"
        text3: "Zoom"
    }

    StackLayoutFocusDistributor {
        id: stack
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: mode.bottom
        anchors.topMargin: 0

        FunctionMenu {
            id: menu
        }

        GraphInterface {
            id: graph
            mode_int: mode.savedIndex
            exprList: def.exprList
            nameList: def.nameList

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            clip: true


            /*Connections {
                target: mainMenu
                onSavedIndexChanged: {
                    if (mainMenu.savedIndex == 1) {
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
