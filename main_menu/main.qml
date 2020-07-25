import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 320
    height: 220

    title: qsTr("Main Menu")

    Rectangle {

        id: rec
        anchors.fill: parent

        Component {
            id: buttonDelegate
            Item {
                width: grid.cellWidth; height: grid.cellHeight
                Column {
                    anchors.fill: parent
                    Image {
                        id: image
                        source: model.icon
                        anchors.horizontalCenter: parent.horizontalCenter
                        width:  48
                        height: width
                    }
                    Text {
                        id: caption
                        height: 26
                        text: model.caption
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pixelSize: 11
                        font.family: "DejaVu Sans Mono"
                        wrapMode: Text.WordWrap
                        width: grid.cellWidth
                        horizontalAlignment: Text.AlignHCenter
                        antialiasing: true
                    }
                }
                Keys.onReturnPressed: launcher.launch(model.command)
            }
        }

        Component {
            id: highlightBar
            Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
        }

        GridView {
            id: grid
            anchors.rightMargin: 12
            anchors.bottomMargin: 24
            anchors.leftMargin: 12
            anchors.topMargin: 24
            anchors.fill: parent

            cellWidth: 74; cellHeight: 86

            property int page: 0

            model: listmodels[page].model
            delegate: buttonDelegate
            highlight: highlightBar
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 0

            focus: true

            Keys.onRightPressed: {
                if((currentIndex == 3 || currentIndex == 7) && (page < 2) && (listmodels[page+1].model.count > 0)) {
                    page++
                    currentIndex -= 3
                    if(currentIndex >= listmodels[page].model.count) {
                        currentIndex -= 4
                    }
                } else if((currentIndex != 3) && (currentIndex != 7) && (count > currentIndex+1)) {
                    currentIndex++
                }
            }

            Keys.onLeftPressed: {
                if((currentIndex == 0 || currentIndex == 4) && (page > 0)) {
                    page--
                    currentIndex += 3
                } else if((currentIndex != 0) && (currentIndex != 4)) {
                    currentIndex--
                }
            }

            Keys.onUpPressed: {
                if(currentIndex > 3) {
                    currentIndex -= 4
                }
            }

            Keys.onDownPressed: {
                if(currentIndex < 8 && count > currentIndex+4) {
                    currentIndex += 4
                }
            }
        }


        PageIndicator {
            id: control
            count: 3
            currentIndex: grid.page

            anchors.bottom: rec.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            delegate: Rectangle {
                    implicitWidth: 10
                    implicitHeight: 10

                    radius: height/3
                    color: index === control.currentIndex ? "black" : "lightgray"
                }
        }
    }

}
