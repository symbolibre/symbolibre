import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.applet 1.0
import org.symbolibre.controls 1.0
import org.symbolibre.catalog 1.0

SLStandardApplet {
    id: app

    CatalogPopup {
        id: catalog
        width: parent.width - 20
        height: parent.height - 20
        anchors.centerIn: Overlay.overlay
        catalogId: "math_fr"
    }
    function openCatalog(callback) {
        catalog.setMenu("root");
        catalog.callback = callback;
        catalog.open();
    }

    Popup {
        id: windowRangePopup
        width: parent.width - 20
        height: parent.height - 20
        anchors.centerIn: Overlay.overlay
        modal: true
        focus: true

        FunctionMenu {
            id: menu
            anchors.fill: parent
            focus: true
            FunctionBar.f5: FunctionKeyModel {
                text: qsTr("Confirm")
                onActivated: windowRangePopup.close()
            }
        }
    }

    SLStackLayout {
        id: stackLayout
        anchors.fill: parent
        currentIndex: 0
        clip: true

        FunctionDefinitionView {
            id: def
            focus: true

            FunctionBar.f4: FunctionKeyModel {
                text: qsTr("Window")
                onActivated: windowRangePopup.open()
            }
            FunctionBar.f5: FunctionKeyModel {
                text: qsTr("Graph")
                onActivated: {
                    stackLayout.currentIndex = 1;
                    stackLayout.children[1].forceActiveFocus();
                }
            }
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

            FunctionBar.f4: FunctionKeyModel {
                text: qsTr("Window")
                onActivated: windowRangePopup.open();
            }
            FunctionBar.f5: FunctionKeyModel {
                text: qsTr("Function\ninput")
                onActivated: {
                    stackLayout.currentIndex = 0;
                    stackLayout.children[0].forceActiveFocus();
                }
            }
        }
    }
}
