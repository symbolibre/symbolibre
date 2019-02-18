import QtQuick 2.4
import QtQuick.Controls 2.2

Item {
    width: 320
    height: 40
    id: selectBar
    property int savedIndex: 0
    property int currentIndex: 0
    property alias rectangle: rectangle

    Rectangle {
        id: rectangle
        color: "#ffffff"
        anchors.bottomMargin: 8
        anchors.fill: parent

        TabButton {
            id: tabButton
            width: parent.width/3
            text: qsTr("Tab Button")
            property int index: 0
            x: 0
            y: 0
            down: selectBar.currentIndex == index
        }

        TabButton {
            id: tabButton2
            width: parent.width/3
            text: qsTr("Tab Button")
            property int index: 1
            x: parent.width/3
            y: 0
            down: selectBar.currentIndex == index
        }

        TabButton {
            id: tabButton3
            width: parent.width/3
            text: qsTr("Tab Button")
            property int index: 2
            x: 2*parent.width/3
            y: 0
            down: selectBar.currentIndex == index
        }
    }

}
