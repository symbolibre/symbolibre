import QtQuick 2.4
import QtQuick.Controls 2.2

TabBar {
    width: parent.width
    height: 40
    id: selectBar
    property string text1 : ""
    property string text2 : ""
    property string text3 : ""

    property color coloration1 : "#0A67AF"
    property color coloration2 : "#053D68"

    TabButton {
        id: tabButton1
        height: parent.height
        text: qsTr(text1)
        bottomPadding: 6
        topPadding: 6
        anchors.top: parent.top
        anchors.topMargin: 0
        property int index: 0
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
        height: parent.height
        text: qsTr(text2)
        anchors.top: parent.top
        anchors.topMargin: 0
        property int index: 1
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

    TabButton {
        id: tabButton3
        height: parent.height
        anchors.top: parent.top
        anchors.topMargin: 0
        property int index: 2
        down: selectBar.currentIndex == index

        background: Rectangle {
            color: tabButton3.down ? coloration1 : coloration2
        }

        contentItem: Text {
            text: qsTr(text3)
            color: "#ffffff"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }

}

/*##^## Designer {
    D{i:2;anchors_y:0}D{i:3;anchors_y:0}D{i:4;anchors_y:0}
}
 ##^##*/
