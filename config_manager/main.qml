import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Window {

    visible: true
    width: 320
    height: 216

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    title: qsTr("Paramètres")

    ColumnFocusDistributor {
        id: column
        spacing: 8
        anchors.fill: parent

        TabBar {
            id: bar
            width: parent.width
            font.pixelSize: 11

            Repeater {
                model: configmodel.model
                TabButton {
                    id: tabButton

                    Component.onCompleted: {
                        if(index == 0) {
                            forceActiveFocus()
                        }
                    }

                    contentItem: Text {
                        text: label
                        font: tabButton.font
                        color: bar.currentIndex == index ? "black" : "white"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    background: Rectangle {
                        color: parent.activeFocus ? "lightblue" : (bar.currentIndex == index ? "white" : "black")
                    }
                }
            }
        }

        StackLayout {
            onFocusChanged: {
                if (focus == true) {
                    rep.forceActiveFocus()
                }
            }

            width: parent.width
            currentIndex: bar.currentIndex
            Repeater {
                onFocusChanged: {
                    if (focus == true) {
                        itemAt(bar.currentIndex).forceActiveFocus()
                    }
                }

                id: rep
                model: configmodel.model

               ColumnFocusDistributor {
                    id: colparams
                    Repeater {
                        model: params
                        Component {
                            Loader {

                                source: switch(type) {
                                    case "choice": return "Choice.qml"
                                    case "slider": return "CustomSlider.qml"
                                    case "bool": return "Boolean.qml"
                                    case "entry": return "Entry.qml"
                                }
                                onLoaded: {
                                    item.props = model
                                    item.init()
                                }
                            }
                        }
                    }
               }
            }

        }
    }
}




















