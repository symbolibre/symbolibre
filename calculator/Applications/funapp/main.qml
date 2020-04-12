import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import "components"

ApplicationWindow {
    id: window

    visible: true
    width: 320
    height: 220
    font.pixelSize: 13
    title: qsTr("Quick Calculator")

    palette.window: "white"
    palette.windowText: "black"
    palette.base: "white"
    palette.alternateBase: "lightgray"
    palette.text: "black"
    palette.buttonText: "white"
    palette.brightText: "white"
    palette.highlight: "#4080c0"
    palette.highlightedText: "white"

    palette.button: "#2b79b5"
    palette.light: "#0a67af"
    palette.midlight: "#4e8fc1"
    palette.dark: "#053d68"
    palette.mid: "#074e84"

    CatalogPopup {
        id: catalog
        x: parent.x
        y: parent.y + parent.height / 6
        width: parent.width
        height: parent.height * 2 / 3
    }
    function openCatalog(callback) {
        catalog.setMenu("root");
        catalog.callback = callback;
        catalog.open();
    }

    Rectangle {
        id: screen
        anchors.fill: parent

        FocusScope {
            id: focusDistributor
            clip: true
            focus: true
            anchors.fill: parent
            SLTabBar {
                id: mainMenu
                x: 0
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: 0
                Repeater {
                    model: ["Functions", "Window", "Graph"]
                    TabButton {
                        text: modelData
                    }
                }
                KeyNavigation.down: stackLayout
            }

            SLStackLayout {
                id: stackLayout
                clip: true
                anchors.top: mainMenu.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                currentIndex: mainMenu.currentIndex

                FunctionDefinitionView {
                    id: def
                    focus: true
                }

                FunctionMenu {
                    id: menu
                }

                GraphInterface {
                    id: graph
                    width: parent.width
                    height: parent.height - 15
                    functions: def.functions
                    clip: true
                    onExit: mainMenu.forceActiveFocus()
                    onVisibleChanged: {
                        if (visible) {
                            setRange(menu.xmin, menu.xmax, menu.ymin, menu.ymax)
                            plot()
                        }
                    }
                    onRangeCom : {
                        menu.xmin = xmin
                        menu.xmax = xmax
                        menu.ymin = ymin
                        menu.ymax = ymax
                    }
                }

                Label {
                    id: xtext
                    visible: graph.focus
                    anchors.bottom: parent.bottom
                    anchors.right: ytext.left
                    width: 75
                    height: 20
                    anchors.bottomMargin: 0
                    text: "X = " + String(graph.cursorX.toFixed(4))
                }

                Label {
                    id: ytext
                    visible: graph.focus
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    width: 75
                    height: 20
                    anchors.bottomMargin: 0
                    text: "Y = " + String(graph.cursorY.toFixed(4))
                }
            }
        }
    }
}
