import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import SLKeyCode 1.0
import "../keyboard"
import "../components"
import QtQuick.Window 2.3
import SLCustomPlotItem 1.0
import etBox 1.0

Rectangle {
    id: screen
    width: 320
    height: 220

    property alias graph: functionGraph.graph

    FocusDistributor {
        id: focusDistributor
        clip: true
        anchors.fill: parent
        SLTabBar {
            focus: true
            id: mainMenu
            x: 0
            width: parent.width
            height: 30
            anchors.top: parent.top
            anchors.topMargin: 0
            Repeater {
                model: ["Functions", "Graph"]
                TabButton {
                    text: modelData
                }
            }
        }

        SLStackLayout {
            id: stackLayoutFocusDistributor
            clip: true
            anchors.top: mainMenu.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 0
            currentIndex: mainMenu.currentIndex

            FunctionDefinitionView {
                id: def
            }

            FunctionGraph {
                id: functionGraph
                functions: def.functions
            }

        }
    }
}


/*##^## Designer {
    D{i:3;anchors_x:0}
}
 ##^##*/
