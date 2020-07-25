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
            property int index: 0
            currentIndex: index

            model: listmodels[page].model
            delegate: buttonDelegate
            highlight: highlightBar
            highlightFollowsCurrentItem: true
            highlightMoveDuration: 0

            focus: true

            Keys.onRightPressed: {
                if((index == 3 || index == 7) && (page < 2) && (listmodels[page+1].model.count > 0)) {
                    page++
                    index -= 3
                    if(index >= listmodels[page].model.count) {
                        index -= 4
                    }
                } else if((index != 3) && (index != 7) && (count > index+1)) {
                    index++
                }
            }

            Keys.onLeftPressed: {
                if((index == 0 || index == 4) && (page > 0)) {
                    page--
                    index += 3
                } else if((index != 0) && (index != 4)) {
                    index--
                }
            }

            Keys.onUpPressed: {
                if(index > 3) {
                    index -= 4
                }
            }

            Keys.onDownPressed: {
                if(index < 8 && count > index+4) {
                    index += 4
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
