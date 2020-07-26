import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.controls 1.0
import org.symbolibre.catalog 1.0

SLStandardApplet {
    id: app

    CatalogPopup {
        id: catalog
        parent: Overlay.overlay
        width: parent.width
        height: parent.height * 2 / 3
        anchors.centerIn: parent
        catalogId: "math_fr"
    }
    function openCatalog(callback) {
        catalog.setMenu("root");
        catalog.callback = callback;
        catalog.open();
    }

    Popup {
        id: windowRangePopup
        parent: Overlay.overlay
        width: parent.width
        height: parent.height * 2 / 3
        anchors.centerIn: parent
        modal: true
        focus: true

        FunctionMenu {
            id: menu
            anchors.fill: parent
            focus: true
            property var functionBarModel:
                [ "F1", "F2", "F3", "F4", qsTr("Valider")]
        }
    }

    SLStackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 0

        FunctionDefinitionView {
            id: def
            focus: true

            property var functionBarModel:
                ["F1", "F2", "F3", qsTr("Fenêtre"), qsTr("Graphe")]
            property var statusBarLabel: "Saisie fonctions"
        }

        FocusScope {
            GraphInterface {
                id: graph
                width: parent.width
                height: parent.height - 20
                focus: true
                functions: def.functions
                clip: true
                Keys.onReturnPressed: mainMenu.forceActiveFocus()
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
                visible: graph.cursorAttached
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
                visible: !graph.cursorAttached
                anchors.bottom: parent.bottom
                anchors.right: ytext.left
                width: 75
                height: 20
                anchors.bottomMargin: 0
                text: "X = " + String(graph.cursorPos.x.toPrecision(4))
            }
            Label {
                id: ytext
                visible: !graph.cursorAttached
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: 75
                height: 20
                anchors.bottomMargin: 0
                text: "Y = " + String(graph.cursorPos.y.toPrecision(4))
            }

            property var statusBarLabel: "Graphe"
            property var functionBarModel:
                ["F1", "F2", "F3", qsTr("Fenêtre"), qsTr("Saisie\nfonctions")]
        }
    }

    Shortcut {
        sequence: "F4"
        onActivated: windowRangePopup.open();
    }
    Shortcut {
        sequence: "F5"
        onActivated: {
            stackLayout.currentIndex ^= 1;
            stackLayout.children[stackLayout.currentIndex].forceActiveFocus();
        }
    }
}
