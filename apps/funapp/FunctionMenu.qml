// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

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

        TextField {
            id: xminField
            focus: true
            text: +xmin.toPrecision(6)
            Layout.fillWidth: true
            KeyNavigation.down: xmaxField
            onActiveFocusChanged: if (!activeFocus) xmin = parseFloat(text)
        }

        Label {
            text: qsTr("Xmax")
        }

        TextField {
            id: xmaxField
            text: +xmax.toPrecision(6)
            Layout.fillWidth: true
            KeyNavigation.down: yminField
            onActiveFocusChanged: if (!activeFocus) xmax = parseFloat(text)
        }

        Label {
            text: qsTr("Ymin")
        }

        TextField {
            id: yminField
            text: +ymin.toPrecision(6)
            Layout.fillWidth: true
            KeyNavigation.down: ymaxField
            onActiveFocusChanged: if (!activeFocus) ymin = parseFloat(text)
        }

        Label {
            text: qsTr("Ymax")
        }

        TextField {
            id: ymaxField
            text: +ymax.toPrecision(6)
            Layout.fillWidth: true
            onActiveFocusChanged: if (!activeFocus) ymax = parseFloat(text)
        }
    }
}
