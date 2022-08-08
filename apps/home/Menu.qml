// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.controls 0.2
import org.symbolibre.keyboard 0.2
import org.symbolibre.util 0.2

GridView {
    id: grid
    anchors.rightMargin: 12
    anchors.bottomMargin: 12
    anchors.leftMargin: 12
    anchors.topMargin: 12
    anchors.fill: parent

    cellWidth: 74; cellHeight: 86

    model: appManager.appsModel
    delegate: buttonDelegate
    highlight: highlightBar
    highlightFollowsCurrentItem: true
    highlightMoveDuration: 0

    focus: true

    // we intercept the home key here so that it does not open the dialog to back to the menu
    Keys.onPressed: event => { if (event.key == SLKey.Home) event.accepted = true }

    Component {
        id: buttonDelegate
        Item {
            width: grid.cellWidth; height: grid.cellHeight
            Column {
                anchors.fill: parent
                Image {
                    id: image
                    source: Fs.staticDataDir() + "/icons/" + (modelData.iconPath || "programs.png")
                    anchors.horizontalCenter: parent.horizontalCenter
                    width:  48
                    height: width
                }
                Text {
                    id: caption
                    height: 26
                    text: modelData.name
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 11
                    wrapMode: Text.WordWrap
                    width: grid.cellWidth
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            TapHandler {
                onPressedChanged: if (pressed) grid.currentIndex = index
                onTapped: window.launch(modelData)
            }

            Keys.onReturnPressed: window.launch(modelData)
        }
    }

    Component {
        id: highlightBar
        Rectangle {
            color: palette.highlight
            radius: 5
        }
    }
}
