import QtQuick 2.4
import QtQuick.Controls 2.2
import "../components"

Rectangle {
    id: rectangle
    width: 320
    height: 170
    property alias focusDistributor : focusDistributor
    property alias xminEntry: xminEntry
    property alias xmaxEntry: xmaxEntry
    property alias yminEntry: yminEntry
    property alias ymaxEntry: ymaxEntry
    property double xmin: 0
    property double ymin: 0
    property double xmax: 0
    property double ymax: 0
    property int vertical_spacing: height/16 // Space between boxes
    property int horizontal_spacing : width / 8
    property int box_height: height/6  // height of boxes
    property int text_width : width / 6


    signal defined(double xmin, double xmax, double ymin, double ymax)

    Text {
        id: xminText
        x: 22
        width: text_width
        height: box_height
        text: qsTr("Xmin")
        anchors.top: parent.top
        anchors.topMargin: vertical_spacing
        anchors.left: parent.left
        anchors.leftMargin: horizontal_spacing
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pointSize: height / 2
    }

    Text {
        id: xmaxText
        x: 22
        width: text_width
        height: box_height
        text: qsTr("Xmax")
        anchors.top: xminText.bottom
        anchors.topMargin: vertical_spacing
        anchors.left: parent.left
        anchors.leftMargin: horizontal_spacing
        verticalAlignment: Text.AlignVCenter
        font.pointSize: height / 2
        horizontalAlignment: Text.AlignHCenter
    }


    Text {
        id: yminText
        x: 22
        y: 98
        width: text_width
        height: box_height
        text: qsTr("Ymin")
        anchors.bottom: ymaxText.top
        anchors.bottomMargin: vertical_spacing
        anchors.left: parent.left
        anchors.leftMargin: horizontal_spacing
        verticalAlignment: Text.AlignVCenter
        font.pointSize: height / 2
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: ymaxText
        x: 22
        y: 138
        width: text_width
        height: box_height
        text: qsTr("Ymax")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: vertical_spacing
        anchors.left: parent.left
        anchors.leftMargin: horizontal_spacing
        verticalAlignment: Text.AlignVCenter
        font.pointSize: height / 2
        horizontalAlignment: Text.AlignHCenter
    }


    FocusDistributor {
        id: focusDistributor
        anchors.fill: parent

        TextFieldCustom {
            id : xminEntry
            x: 135
            width: parent.width / 4
            height: box_height
            anchors.top: parent.top
            anchors.topMargin: vertical_spacing
            anchors.right: parent.right
            anchors.rightMargin: horizontal_spacing * 2
        }

        TextFieldCustom {
            id : xmaxEntry
            x: 135
            width: parent.width / 4
            height: box_height
            anchors.top: xminEntry.bottom
            anchors.topMargin: vertical_spacing
            anchors.right: parent.right
            anchors.rightMargin: horizontal_spacing * 2
        }

        TextFieldCustom {
            id: yminEntry
            x: 135
            y: 98
            width: parent.width / 4
            height: box_height
            anchors.bottom: ymaxEntry.top
            anchors.bottomMargin: vertical_spacing
            anchors.right: parent.right
            anchors.rightMargin: horizontal_spacing * 2
        }

        TextFieldCustom {
            id : ymaxEntry
            x: 135
            y: 138
            width: parent.width / 4
            height: box_height
            anchors.bottom: parent.bottom
            anchors.bottomMargin: vertical_spacing
            anchors.right: parent.right
            anchors.rightMargin: horizontal_spacing * 2
        }
    }
}


/*##^## Designer {
    D{i:4;anchors_y:8}D{i:6;anchors_y:48}
}
 ##^##*/
