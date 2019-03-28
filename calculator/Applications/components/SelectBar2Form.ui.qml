import QtQuick 2.4
import QtQuick.Controls 2.2

// Select Bar with only two items, temporary

Item {
    width: 320
    height: 40
    id: selectBar
    property int savedIndex: 0
    property int currentIndex: 0
    property alias rectangle: rectangle
    property string text1 : ""
    property string text2 : ""

    property color coloration1 : "#0A67AF"
    property color coloration2 : "#053D68"

    Rectangle {
        id: rectangle
        width: parent.width
        height: parent.height
        color: "#ffffff"

        TabButton {
            id: tabButton1
            width: parent.width/2 - 1
            height: parent.height
            text: qsTr(text1)
            bottomPadding: 6
            topPadding: 6
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left : parent.left
            property int index: 0
            x: 0
            down: selectBar.currentIndex == index

            background: Rectangle {
                color: tabButton1.down ? coloration1 : coloration2
            }

            contentItem: Text {
                text: qsTr(text1)
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }

        TabButton {
            id: tabButton2
            width: parent.width/2 - 1
            height: parent.height
            text: qsTr(text2)
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 0
            property int index: 1
            x: parent.width/3
            down: selectBar.currentIndex == index

            background: Rectangle {
                color: tabButton2.down ? coloration1 : coloration2
            }

            contentItem: Text {
                text: qsTr(text2)
                color: "#ffffff"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }

}

