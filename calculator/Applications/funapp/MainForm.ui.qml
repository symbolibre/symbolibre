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

    property alias graph: functionGraph.graph

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
    D{i:7;anchors_width:229;anchors_x:65}D{i:9;anchors_width:229;anchors_x:65}D{i:11;anchors_width:229;anchors_x:65}
D{i:13;anchors_width:320}D{i:3;anchors_x:0}
}
 ##^##*/
