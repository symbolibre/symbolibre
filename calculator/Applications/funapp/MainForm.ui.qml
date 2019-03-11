import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"
import "../components"
import QtQuick.Window 2.3
import SLCustomPlotItem 1.0

Rectangle {
    id: screen
    width: 320
    height: 220

    FocusDistributor {
        id: focusDistributor
        x: 0
        y: 0
        clip: true
        anchors.fill: parent
        SelectBar {
            id: mainMenu
            x: 0
            width: parent.width
            height: 30
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.preferredHeight: 30
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            transformOrigin: Item.Center
            text1: "Functions"
            text3: "Table"
            text2: "Graph"
        }

        StackLayoutFocusDistributor {
            id: stackLayoutFocusDistributor
            clip: true
            anchors.top: mainMenu.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 0
            currentIndex: mainMenu.savedIndex
            Layout.fillWidth: true
            Layout.fillHeight: true
            FocusDistributor {
                id: focusDistributor2
                width: 320
                height: 200
                clip: true
                Layout.fillWidth: true
                Layout.fillHeight: true
                HorizontalFocusDistributor {
                    id: horizontalFocusDistributor1
                    anchors.fill: parent
                    TextFieldCustom {
                        id: textFieldCustom6
                        y: 20
                        width: 51
                        height: 39
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        transformOrigin: Item.Center
                        initialText: "f(x)"
                    }

                    TextFieldCustom {
                        id: textFieldCustom7
                        x: 65
                        y: 20
                        width: 229
                        height: 39
                        initialText: ""
                    }

                    TextFieldCustom {
                        id: textFieldCustom8
                        y: 75
                        width: 51
                        height: 39
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        initialText: "g(x)"
                    }

                    TextFieldCustom {
                        id: textFieldCustom9
                        x: 65
                        y: 75
                        width: 229
                        height: 39
                        initialText: ""
                    }

                    TextFieldCustom {
                        id: textFieldCustom10
                        y: 126
                        width: 51
                        height: 39
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        initialText: "h(x)"
                    }

                    TextFieldCustom {
                        id: textFieldCustom11
                        x: 65
                        y: 126
                        width: 229
                        height: 39
                        initialText: ""
                    }
                }
            }

            FocusDistributor {
                id: focusDistributor3
                clip: true
                scale: 1
                Layout.preferredHeight: -1
                Layout.preferredWidth: -1
                Layout.minimumHeight: 0
                anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true
                SelectBar {
                    id: mode
                    width: 320
                    height: 20
                    clip: true
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    text1: "Axis"
                    text2: "Cursor"
                    text3: "Zoom"
                }
                GraphInterface {
                    id : graph
                    width: 320
                    clip: true
                    mode_int: mode.savedIndex
                    exprList: horizontalFocusDistributor1.exprList
                    nameList: horizontalFocusDistributor1.nameList
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 0
                    anchors.top: mode.bottom
                    anchors.topMargin: 0
                    Connections {
                        target: mainMenu
                        onSavedIndexChanged: {
                            if (mainMenu.savedIndex == 1) {
                                graph.plot()
                            }
                        }
                    }
                }
            }
        }
    }
}


/*##^## Designer {
    D{i:3;anchors_x:0}
}
 ##^##*/
