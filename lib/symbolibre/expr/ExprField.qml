// Copyright 2018-2020 Symbolibre authors <https://symbolibre.org>
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.applet 0.2
import org.symbolibre.expr 0.2
import org.symbolibre.controls 0.2
import org.symbolibre.keyboard 0.2
import "editiontree.js" as ET

FocusScope {
    id: root
    implicitWidth: expr.implicitWidth + 4
    implicitHeight: expr.implicitHeight + 4
    property bool combineExprControls: false
    property alias text: expr.text
    property alias json: expr.json
    property alias font: expr.font

    property bool permanentEditFocus: false

    function insertJson(json) {
        return expr.insertJson(json);
    }

    function clear() {
        json = JSON.stringify([""])
    }

    Keys.onPressed: event => {
        if (permanentEditFocus) return;

        // TODO: Filter out exactly the keys unused by keyPressHandler()
        var modifiers = [
            Qt.Key_Shift, Qt.Key_Control, Qt.Key_Alt, Qt.Key_Meta,
            Qt.Key_AltGr, Qt.Key_Super_L, Qt.Key_Super_R, Qt.Key_Menu ];
        if (modifiers.includes(event.key)) return;

        // Enter and Return are used to enter or leave the field
        if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
            expr.focus = !expr.focus;
            event.accepted = true;
            return;
        }

        // Escape leaves the field
        if (event.key === Qt.Key_Escape) {
            expr.focus = false;
            event.accepted = true;
            return;
        }

        // Delete when field is unfocused clears it
        if (event.key === Qt.Key_Delete && !expr.focus) {
            expr.clear();
            expr.focus = true;
            event.accepted = true;
            return;
        }

        // Left and right while the field is unfocused drop the cursor at the
        // end or start. Any other key starts edition immediately at the end.
        if (!expr.focus) {
            expr.focus = true;

            if (event.key === Qt.Key_Left) {
                expr.dropCursorRight();
            }
            else if (event.key === Qt.Key_Right) {
                expr.dropCursorLeft();
            }
            else {
                expr.dropCursorRight();
                expr.keyPressHandler(event);
            }
        }
    }

    // Stop editing whenever the ExprField loses focus
    onActiveFocusChanged: {
        expr.focus = permanentEditFocus;
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        border.width: root.activeFocus ? 2 : 1
        color: palette.base
        border.color: root.activeFocus ? palette.highlight : palette.mid

        Expr {
            id: expr
            focus: permanentEditFocus
            x: parent.x + 2
            y: parent.y + 2
            width: parent.width - 4
            height: parent.height - 4

            // Allow the ExprField parent to specify whether to combine
            FunctionBar.combine: combineExprControls

            FunctionBar.f1: FunctionKeyModel {
                text: qsTr("Catalog")
                onActivated: app.openCatalog(function (str) {
                    expr.insertJson(str);
                    expr.forceActiveFocus();
                });
            }
            FunctionBar.f2: FunctionKeyModel {
                text: qsTr("Clear")
                onActivated: expr.clear();
            }

            function keyPressHandler(event) {
                function insert(data) {
                    insertJson(JSON.stringify(data))
                }
                function insertText(text) {
                    insertJson(JSON.stringify([text]))
                }

                // Special keys
                if (event.modifiers !== Qt.NoModifier && event.modifiers !== Qt.ShiftModifier)
                    return;

                if (event.key === Qt.Key_Backspace)   expr.deleteChar();
                else if (event.key === Qt.Key_Delete) expr.deleteChar();
                else if (event.key === SLKey.Delete)  expr.deleteChar();

                else if (event.key === Qt.Key_ParenLeft)  insert(ET.lparen());
                else if (event.key === Qt.Key_ParenRight) insert(ET.rparen());

                else if(event.key === Qt.Key_Equal) insertText(":=");

                // Operators
                else if (event.key === Qt.Key_Asterisk) insert(ET.op("*"));
                else if (event.key === Qt.Key_Slash)    insert(ET.frac([""],[""]));
                else if (event.key === Qt.Key_Plus)     insert(ET.op("+"));
                else if (event.key === Qt.Key_Minus)    insert(ET.op("-"));

                // Variables (alphabetic keys) */
                else if (event.key >= Qt.Key_A && event.key <= Qt.Key_Z) {
                    insertText(event.text);
                    // FIXME giac interprets (A)(B) as a function call, so this is disabled for now
                    /*if(event.text.toLowerCase() === event.text)
                      insertText(event.text);
                    else
                      insert(ET.variable(event.text));*/
                }

                // Trigonometry
                else if (event.key === SLKey.Sin)
                    insertText("sin(");
                else if (event.key === SLKey.Cos)
                    insertText("cos(");
                else if (event.key === SLKey.Tan)
                    insertText("tan(");

                // Exponentiation
                else if (event.key === SLKey.Square)
                    insert(["", ET.pow(["2"]), ET.set_cursor("")]);
                else if (event.key === SLKey.Sqrt)
                    insert(ET.sqrt([""]));
                else if (event.key === SLKey.LogE)
                    insertText("ln(");
                else if (event.key === SLKey.Exp)
                    insert(["e", ET.set_cursor(ET.pow([""])), ""]);
                else if (event.key === SLKey.Log10)
                    insertText("log(");
                else if (event.key === SLKey.Pow)
                    insert(ET.set_cursor(ET.pow([""])));

                else if (event.text)
                    insertText(event.text);

                else
                    return;

                event.accepted = true;
            }

            Keys.onPressed: event => {
                if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return ||
                    event.key === Qt.Key_Escape) return;

                keyPressHandler(event);
            }
        }
    }
}
