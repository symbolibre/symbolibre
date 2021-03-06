// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import org.symbolibre.controls 0.2
import org.symbolibre.applet 0.2
import org.symbolibre.keyboard 0.2

FocusScope {
    property alias active: active
    property alias exprfield: exprfield

    id: root
    width: parent.width
    height: exprfield.height
    RowLayout {
        id: row
        anchors.fill: parent
        CheckBox {
            id: active
            checked: model.active
        }
        Label {
            id: name
            text: model.name + "(x) = "
            color: model.color
        }
        ExprField {
            id: exprfield
            Layout.fillWidth: true
            Layout.maximumHeight: 120
            json: model.expr
            onJsonChanged: model.expr = json
            focus: true

            FunctionBar.combine: true
            FunctionBar.f1: FunctionKeyModel {
                text: model.active ? qsTr("Hide") : qsTr("Show")
                onActivated: model.active = !model.active;
            }
        }
    }

    Binding {
        target: model
        property: "active"
        value: active.checked
    }
}
