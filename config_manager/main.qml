import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Extras 1.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3

Window {
    id: window
    visible: true
    width: 320
    height: 216

    maximumHeight: height
    maximumWidth: width

    minimumHeight: height
    minimumWidth: width

    title: qsTr("Paramètres")

    ColumnLayout {
        id: column
        spacing: 8
        anchors.fill: parent

        TabBar {
            id: bar
            Layout.fillWidth: true
            font.pixelSize: 13
            focus: true
            KeyNavigation.down: tabStack.children[tabStack.currentIndex]

            Repeater {
                model: configmodel.model
                TabButton {
                    id: tabButton
                    focus: true
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
            id: tabStack
            Layout.fillWidth: true
            currentIndex: bar.currentIndex
            Repeater {
                id: rep
                model: configmodel.model

                ListView {
                    id: colparams
                    focus: true
                    model: params
                    delegate: Component {
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




















