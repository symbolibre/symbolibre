import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"

Rectangle {
    width: 320
    height: 240 * 2
    property alias keyboard: keyboard

    Rectangle {
        id: pad
        height: 240
        width: 320
        x: 0
        y: 240
        color: "black"

        SLKeyBoard {
            id: keyboard
        }
    }


    ColumnLayout {
        id: columnLayout
        x: 0
        y: 0
        width: 320
        height: 240

        TabBar {
            id: tabBar
            anchors.fill: parent

            TabButton {
                id: tabButton1
                text: qsTr("Tab Button")
            }

            TabButton {
                id: tabButton
                x: 106
                y: 0
                text: qsTr("Tab Button")
            }

            TabButton {
                id: tabButton2
                text: qsTr("Tab Button")
            }
        }

        StackLayout {
            id: stackLayout
            width: 320
            height: 199

            TextEdit {
                id: textEdit
                width: 80
                height: 20
                text: qsTr("Edit")
                font.pixelSize: 12

                RowLayout {
                    id: rowLayout
                    x: 0
                    y: 42
                    width: 320
                    height: 47
                }
            }

            TextEdit {
                id: textEdit1
                width: 80
                height: 20
                text: qsTr("Graph")
                font.pixelSize: 12
            }

            TextEdit {
                id: textEdit2
                width: 80
                height: 20
                text: qsTr("Table")
                font.pixelSize: 12
            }
        }



    }
}

/*##^## Designer {
    D{i:86;anchors_height:0;anchors_width:320}
}
 ##^##*/
