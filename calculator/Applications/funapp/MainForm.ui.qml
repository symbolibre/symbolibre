import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"
import QtQuick.Window 2.3

Rectangle {
    id: screen
    width: 320
    height: 240

    ColumnLayoutFocusDistributor {
        id: columnLayoutFocusDistributor
        anchors.fill: parent

        SelectBar {
            id: selectBar
            x: 1
            y: 1
            width: parent.width
        }

        StackLayoutFocusDistributor {
            id: stackLayoutFocusDistributor
            x: 0
            y: 45
            currentIndex: selectBar.savedIndex

            ColumnLayoutFocusDistributor {
                id: columnLayoutFocusDistributor1
                width: 320
                height: 146

                TextField {
                    id: textField
                    text: qsTr("Text Field")
                }

                ColumnLayoutFocusDistributor {
                    id: columnLayoutFocusDistributor2
                    width: 320
                    height: 146
                    TextField {
                        id: textField1
                        text: qsTr("Text Field")
                    }

                    TextField {
                        id: textField3
                        text: qsTr("Text Field")
                    }
                }

                TextField {
                    id: textField2
                    text: qsTr("Text Field")
                }

            }

            Text {
                id: text1
                text: qsTr("Fun")
                font.pixelSize: 12
            }

            Text {
                id: text2
                text: qsTr("Table")
                font.pixelSize: 12
            }
        }

    }



}

/*##^## Designer {
    D{i:1;anchors_height:240;anchors_width:320;anchors_x:0;anchors_y:0}
}
 ##^##*/
