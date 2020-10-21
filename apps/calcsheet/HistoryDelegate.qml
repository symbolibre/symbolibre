// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Layouts 1.12
import org.symbolibre.keyboard 1.0
import org.symbolibre.expr 1.0
import org.symbolibre.style 1.0

FocusScope {
    width: (parent ? parent.width : 1)
    height: column.height
    ColumnLayout {
        id: column
        width: parent.width
        spacing: 0
        Rectangle {
            color: "white"
            Layout.fillWidth: true
            Layout.preferredHeight: sourceExpr.implicitHeight
            Layout.maximumHeight: 120
            Expr {
                id: sourceExpr
                height: parent.height
                horizontalAlignment: Expr.AlignLeft
                fillColor: activeFocus ? "lightblue" : "white"
                font.family: SLStyle.mathFont
                json: model.source
                KeyNavigation.down: resultExpr
                Keys.onPressed: {
                    if (event.key == SLKey.Enter) {
                        expr.json = json;
                        expr.forceActiveFocus();
                        event.accepted = true;
                    }
                }
            }
        }

        Rectangle {
            color: "lightgray"
            Layout.fillWidth: true
            Layout.preferredHeight: resultExpr.implicitHeight
            Layout.maximumHeight: 120
            Expr {
                id: resultExpr
                height: parent.height
                anchors.right: parent.right
                horizontalAlignment: Expr.AlignRight
                fillColor: activeFocus ? "lightblue" : "lightgray"
                font.family: SLStyle.mathFont
                json: model.result
                focus: true
                Keys.onPressed: {
                    if (event.key == SLKey.Enter) {
                        expr.json = json;
                        expr.forceActiveFocus();
                        event.accepted = true;
                    }
                }
            }
        }
    }
    Keys.onPressed: {
        if (event.key == SLKey.Delete) {
            historyModel.remove(index);
            event.accepted = true;
        }
    }
}
