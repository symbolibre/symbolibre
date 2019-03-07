import QtQuick 2.4
import QtQuick.Controls 2.2

Item {
    width: 320
    height: 40
    id: selectBar
    property int savedIndex: 0
    property int currentIndex: 0
    property alias rectangle: rectangle
    property string text1 : ""
    property string text2 : ""
    property string text3 : ""

    property color coloration1 : "#0A67AF"
    property color coloration2 : "#053D68"

    Rectangle {
        id: rectangle
        color: "#0A67AF"
        anchors.bottomMargin: 0
        anchors.fill: parent

        TabButton {
            id: tabButton1
            width: parent.width/3
            text: qsTr(text1)
            anchors.verticalCenter: parent.verticalCenter
            anchors.top: parent.top
            anchors.topMargin: 0
            property int index: 0
            x: 0
            down: selectBar.currentIndex == index

            background: Rectangle {
                color: tabButton1.down ? coloration1 : coloration2
            }
        }

        TabButton {
            id: tabButton2
            width: parent.width/3
            text: qsTr(text2)
            anchors.verticalCenter: parent.verticalCenter
            anchors.top: parent.top
            anchors.topMargin: 0
            property int index: 1
            x: parent.width/3
            down: selectBar.currentIndex == index

            background: Rectangle {
                color: tabButton2.down ? coloration1 : coloration2
            }
        }

        TabButton {
            id: tabButton3
            width: parent.width/3
            text: qsTr(text3)
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            property int index: 2
            x: 2*parent.width/3
            down: selectBar.currentIndex == index

            background: Rectangle {
                color: tabButton3.down ? coloration1 : coloration2
            }
        }
    }

}

/*##^## Designer {
    D{i:2;anchors_y:0}D{i:3;anchors_y:0}D{i:4;anchors_y:0}
}
 ##^##*/
