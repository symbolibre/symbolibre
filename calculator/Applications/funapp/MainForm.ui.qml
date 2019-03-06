import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"
import "../components"
import QtQuick.Window 2.3

Rectangle {
    id: screen
    width: 320
    height: 220

    ColumnLayoutFocusDistributor {
        id: columnLayoutFocusDistributor
        spacing: 0
        anchors.fill: parent

        SelectBar {
            id: selectBar
            x: 0
            y: 0
            width: parent.width
            Layout.fillWidth: true
            transformOrigin: Item.Center
        }

        StackLayoutFocusDistributor {
            id: stackLayoutFocusDistributor
            x: 0
            y: 45
            Layout.fillHeight: true
            Layout.fillWidth: true
            currentIndex: selectBar.savedIndex

            FocusDistributor {
                id: focusDistributor1
                width: 320
                height: 199
                Layout.fillHeight: true
                Layout.fillWidth: true


                HorizontalFocusDistributor {
                    id: horizontalFocusDistributor
                    x: 0
                    y: 0

                    TextFieldCustom {
                        id: textFieldCustom
                        x: 8
                        y: 19
                        width: 51
                        height: 39
                        initialText: "f(x)"
                    }

                    TextFieldCustom {
                        id: textFieldCustom1
                        x: 65
                        y: 19
                        width: 229
                        height: 39
                        initialText: ""
                    }

                }

                HorizontalFocusDistributor {
                    id: horizontalFocusDistributor1
                    x: 0
                    y: 50
                    TextFieldCustom {
                        id: textFieldCustom2
                        x: 8
                        y: 19
                        width: 51
                        height: 39
                        initialText: "g(x)"
                    }

                    TextFieldCustom {
                        id: textFieldCustom3
                        x: 65
                        y: 19
                        width: 229
                        height: 39
                        initialText: ""
                    }
                }

                HorizontalFocusDistributor {
                    id: horizontalFocusDistributor2
                    x: 0
                    y: 100
                    TextFieldCustom {
                        id: textFieldCustom4
                        x: 8
                        y: 19
                        width: 51
                        height: 39
                        initialText: "h(x)"
                    }

                    TextFieldCustom {
                        id: textFieldCustom5
                        x: 65
                        y: 19
                        width: 229
                        height: 39
                        initialText: ""
                    }
                }
            }

            FocusDistributor {
                id: focusDistributor
                width: 320
                height: 199
                Layout.fillHeight: true
                Layout.fillWidth: true

                SelectBar {
                    id: selectBar1
                    width: 320
                    height: 16
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                }
            }

        }
    }
}
