import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.11
import "../../lib/controls"
import "../../lib/editiontree"

FocusScope {
    property alias active: active
    property alias exprfield: exprfield

    id: root
    width: parent.width
    height: exprfield.height
    RowLayout {
        id: row
        anchors.fill: parent
        SLCheckBox {
            id: active
            checked: model.active
            KeyNavigation.right: exprfield
            // FIXME is there a simpler/cleaner way to do this?
            Keys.onDownPressed: {
                root.ListView.view.incrementCurrentIndex();
                root.ListView.view.currentItem.active.forceActiveFocus()
            }
            Keys.onUpPressed: {
                if (index == 0) {
                    event.accepted = false
                } else {
                    root.ListView.view.decrementCurrentIndex();
                    root.ListView.view.currentItem.active.forceActiveFocus()
                }
            }
        }
        Label {
            id: name
            text: model.name + "(x) = "
            color: model.color
            KeyNavigation.right: exprfield
        }
        ExprField {
            id: exprfield
            Layout.fillWidth: true
            json: model.expr
            onJsonChanged: model.expr = json
            focus: true
            Keys.onDownPressed: {
                root.ListView.view.incrementCurrentIndex();
                root.ListView.view.currentItem.exprfield.forceActiveFocus()
            }
            Keys.onUpPressed: {
                if (index == 0) {
                    event.accepted = false
                } else {
                    root.ListView.view.decrementCurrentIndex();
                    root.ListView.view.currentItem.exprfield.forceActiveFocus()
                }
            }
            Keys.onPressed: {
                if (event.key == Qt.Key_F1)
                    window.openCatalog(function(str) {
                        exprfield.insertJson(str);
                    });
            }
        }
    }

    Binding {
        target: model
        property: "active"
        value: active.checked
    }
}
