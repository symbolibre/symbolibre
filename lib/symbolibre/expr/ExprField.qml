import QtQuick 2.8
import QtQuick.Controls 2.2
import SLKeyCode 1.0
import org.symbolibre.expr 1.0
import "editiontree.js" as ET

// adapted from CalcSheet

FocusScope {
    id: root
    implicitWidth: expr.implicitWidth + 4
    implicitHeight: expr.implicitHeight + 4
    property alias text: expr.text
    property alias json: expr.json

    function insertJson(json) {
        return expr.insertJson(json);
    }

    function clear() {
        json = JSON.stringify([""])
    }

    Rectangle {
        id: rect
        anchors.fill: parent
        border.width: root.activeFocus ? 2 : 1
        color: palette.base
        border.color: root.activeFocus ? palette.highlight : palette.mid

        Expr {
            id: expr
            focus: true
            x: parent.x + 2
            y: parent.y + 2
            width: parent.width - 4
            height: parent.height - 4

            Keys.onPressed: {
                function insert(data) {
                    insertJson(JSON.stringify(data))
                }
                function insertText(text) {
                    insertJson(JSON.stringify([text]))
                }

                /* Special keys */
                if(event.key === Qt.Key_Backspace) expr.deleteChar();
                if(event.key === Qt.Key_Delete) expr.deleteChar();

                /* Operator keys */
                if(event.key === Qt.Key_Asterisk)  insert(ET.op("*"));
                if(event.key === Qt.Key_Slash)     insert(ET.frac([""],[""]));
                if(event.key === Qt.Key_Plus)      insert(ET.op("+"));
                if(event.key === Qt.Key_Minus)     insert(ET.op("-"));

                /* Raw input keys */
                if(event.key >= Qt.Key_A && event.key <= Qt.Key_Z && event.text) {
                    insertText(event.text);
                    // FIXME giac interprets (A)(B) as a function call, so this is disabled for now
                    /*if(event.text.toLowerCase() === event.text)
                      insertText(event.text);
                    else
                      insert(ET.variable(event.text));*/
                }

                if(event.key >= Qt.Key_0 && event.key <= Qt.Key_9)
                    insertText(event.text);

                /* Punctuation */
                if(event.key === Qt.Key_ParenLeft)  insert(ET.lparen());
                if(event.key === Qt.Key_ParenRight) insert(ET.rparen());

                if(event.key === Qt.Key_Equal)
                    insertText(":=");

                if(event.key === Qt.Key_Comma ||
                   event.key === Qt.Key_Period ||
                   event.key === Qt.Key_Space)
                   insertText(event.text);

                /* New keys */ // TODO : dispatch when debugged

                if (event.key === Qt.Key_F15 && (event.modifiers & Qt.ShiftModifier))
                    insert(ET.sqrt([""]));

                if (event.key === Qt.Key_F10 && (event.modifiers & Qt.ShiftModifier))
                    insertText("asin");
                else if (event.key === Qt.Key_F10)
                    insertText("sin");
                if (event.key === Qt.Key_F11 && (event.modifiers & Qt.ShiftModifier))
                    insertText("acos");
                else if (event.key === Qt.Key_F11)
                    insertText("cos");
                if (event.key === Qt.Key_F12 && (event.modifiers & Qt.ShiftModifier))
                    insertText("atan");
                else if (event.key === Qt.Key_F12)
                    insertText("tan");
                if (event.key === Qt.Key_F13)
                    insertText("π");

                // pow, exponential
                if (event.key === Qt.Key_F16 && (event.modifiers & Qt.ShiftModifier))
                    insertText("ln");
                else if (event.key === Qt.Key_F16)
                    insert(["e", ET.set_cursor(ET.pow([""])), ""]);
                if (event.key === Qt.Key_F17 && (event.modifiers & Qt.ShiftModifier))
                    insertText("log");
                else if (event.key === Qt.Key_F17)
                    insert(ET.set_cursor(ET.pow([""])));
                if(event.key === Qt.Key_sterling)
                    insert(ET.set_cursor(ET.pow([""])));
                if (event.key === Qt.Key_F21)
                    insert(["10", ET.set_cursor(ET.pow([""])), ""]);
            }
        }
    }
}
