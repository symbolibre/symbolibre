import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0
import "components"

FocusScope {
    id: root

    property double xmin: -10
    property double xmax: 10
    property double ymin: -10
    property double ymax: 10


    GridLayout {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 8
        columns: 2

        Label {
            text: qsTr("Xmin")
        }

        SLTextField {
            id: xminField
            focus: true
            text: +xmin.toPrecision(6)
            Layout.fillWidth: true
            KeyNavigation.down: xmaxField
            onEditFocusChanged: if (!editFocus) xmin = parseFloat(text)
        }

        Label {
            text: qsTr("Xmax")
        }

        SLTextField {
            id: xmaxField
            text: +xmax.toPrecision(6)
            Layout.fillWidth: true
            KeyNavigation.down: yminField
            onEditFocusChanged: if (!editFocus) xmax = parseFloat(text)
        }

        Label {
            text: qsTr("Ymin")
        }

        SLTextField {
            id: yminField
            text: +ymin.toPrecision(6)
            Layout.fillWidth: true
            KeyNavigation.down: ymaxField
            onEditFocusChanged: if (!editFocus) ymin = parseFloat(text)
        }

        Label {
            text: qsTr("Ymax")
        }

        SLTextField {
            id: ymaxField
            text: +ymax.toPrecision(6)
            Layout.fillWidth: true
            onEditFocusChanged: if (!editFocus) ymax = parseFloat(text)
        }
    }
}
