import QtQuick 2.12
import QtQuick.Layouts 1.11
import "components"
import etBox 1.0

FocusScope {
    width: parent.width
    height: column.height
    ColumnLayout {
        id: column
        width: parent.width
        spacing: 0
        Rectangle {
            color: sourceExpr.fillColor
            width: parent.width
            Layout.preferredHeight: sourceExpr.implicitHeight
            Layout.maximumHeight: 120
            Expr {
                id: sourceExpr
                width: parent.width
                height: parent.height
                horizontalAlignment: ETBox.AlignLeft
                fillColor: activeFocus ? "lightblue" : "white"
                json: model.sourceJson
                KeyNavigation.down: resultExpr
                Keys.onPressed: {
                    if (event.key == Qt.Key_Return) {
                        expr.json = json;
                        expr.forceActiveFocus();
                        event.accepted = true;
                    }
                }
            }
        }

        Rectangle {
            color: resultExpr.color
            width: parent.width
            Layout.preferredHeight: resultExpr.implicitHeight
            Layout.maximumHeight: 120
            Expr {
                id: resultExpr
                width: parent.width
                height: parent.height
                horizontalAlignment: ETBox.AlignRight
                fillColor: activeFocus ? "lightblue" : "lightgray"
                json: model.resultJson
                focus: true
                Keys.onPressed: {
                    if (event.key == Qt.Key_Return) {
                        expr.json = json;
                        expr.forceActiveFocus();
                        event.accepted = true;
                    }
                }
            }
        }
    }
    Keys.onPressed: {
        if (event.key == Qt.Key_Delete) {
            historyModel.remove(index);
            event.accepted = true;
        }
    }
}
