import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.symbolibre.mathrenderer 1.0
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
        x: 0
        y: 0
        clip: true
        anchors.fill: parent
        SelectBar2 {
            focus: true
            id: mainMenu
            x: 0
            width: parent.width
            height: 30
            anchors.top: parent.top
            anchors.topMargin: 0
            text1: "Functions"
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

            FunctionDefinition {
                id: def
                anchors.fill: parent
            }

            FunctionGraph {
                id: functionGraph
                anchors.fill: parent
            }

        }
    }
}


/*##^## Designer {
    D{i:3;anchors_x:0}
}
 ##^##*/
