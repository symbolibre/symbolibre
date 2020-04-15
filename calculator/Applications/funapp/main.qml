import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import "components"

SLWindow {
    id: window

    visible: true
    width: 320
    height: 220
    title: qsTr("Quick Calculator")

    CatalogPopup {
        id: catalog
        x: parent.x
        y: parent.y + parent.height / 6
        width: parent.width
        height: parent.height * 2 / 3
        catalogId: "math_fr"
    }
    function openCatalog(callback) {
        catalog.setMenu("root");
        catalog.callback = callback;
        catalog.open();
    }

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

        FocusScope {
            GraphInterface {
                id: graph
                width: parent.width
                height: parent.height - 20
                focus: true
                functions: def.functions
                clip: true
                onExit: mainMenu.forceActiveFocus()
                onVisibleChanged: {
                    if (visible) {
                        setRange(menu.xmin, menu.xmax, menu.ymin, menu.ymax)
                        plot()
                    }
                }
                onViewChanged: {
                    menu.xmin = view.x
                    menu.xmax = view.x + view.width
                    menu.ymin = view.y
                    menu.ymax = view.y + view.height
                }
            }

            Label {
                id: ftext
                visible: graph.mode == "Cursor"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                height: 20
                text: {
                    graph.selectedCurve ?
                        graph.selectedCurve +
                        "(" + String(graph.cursorPos.x.toPrecision(4)) + ") = " +
                        String(graph.cursorPos.y.toPrecision(4)) : ""
                }
            }

            Label {
                id: xtext
                visible: graph.mode == "Zoom"
                anchors.bottom: parent.bottom
                anchors.right: ytext.left
                width: 75
                height: 20
                anchors.bottomMargin: 0
                text: "X = " + String(graph.cursorPos.x.toPrecision(4))
            }
            Label {
                id: ytext
                visible: graph.mode == "Zoom"
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: 75
                height: 20
                anchors.bottomMargin: 0
                text: "Y = " + String(graph.cursorPos.y.toPrecision(4))
            }
        }

    }
}
