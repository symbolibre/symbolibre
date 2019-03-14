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

    signal defined(double xmin, double xmax, double ymin, double ymax)

    Text {
        id: xminText
        x: 22
        width: 83
        height: 25
        text: qsTr("Xmin")
        anchors.top: parent.top
        anchors.topMargin: 15
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 20
    }

    Text {
        id: xmaxText
        x: 22
        width: 83
        height: 25
        text: qsTr("Xmax")
        anchors.top: parent.top
        anchors.topMargin: 55
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
    }


    Text {
        id: yminText
        x: 22
        y: 98
        width: 83
        height: 25
        text: qsTr("Ymin")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 55
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
    }

    Text {
        id: ymaxText
        x: 22
        y: 138
        width: 83
        height: 25
        text: qsTr("Ymax")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
    }


    FocusDistributor {
        id: focusDistributor
        anchors.fill: parent

        TextFieldCustom {
            id : xminEntry
            x: 135
            width: 110
            height: 25
            anchors.top: parent.top
            anchors.topMargin: 15
        }

        TextFieldCustom {
            id : xmaxEntry
            x: 135
            width: 110
            height: 25
            anchors.top: parent.top
            anchors.topMargin: 55
        }

        TextFieldCustom {
            id: yminEntry
            x: 135
            y: 98
            width: 110
            height: 25
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 55
        }

        TextFieldCustom {
            id : ymaxEntry
            x: 135
            y: 138
            width: 110
            height: 25
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15
        }
    }
}


/*##^## Designer {
    D{i:4;anchors_y:8}D{i:6;anchors_y:48}
}
 ##^##*/
