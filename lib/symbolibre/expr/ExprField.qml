import QtQuick 2.12
import QtQuick.Controls 2.5
import org.symbolibre.expr 1.0
import org.symbolibre.keyboard 1.0
import "editiontree.js" as ET

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

                // Special keys
                if (event.modifiers !== Qt.NoModifier && event.modifiers !== Qt.ShiftModifier)
                    return;

                // Enter, Return
                if (event.text.includes("\n") || event.text.includes("\r"))
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
        }
    }
}
