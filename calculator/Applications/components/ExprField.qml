import QtQuick 2.8
import QtQuick.Controls 2.2
import SLKeyCode 1.0
import etBox 1.0

// adapted from CalcSheet

FocusScope {
    id: root
    implicitWidth: etbox.implicitWidth
    implicitHeight: etbox.implicitHeight
    property string text: etbox.text

    Rectangle {
        anchors.fill: parent
        border.width: root.activeFocus ? 2 : 1
        color: palette.base
        border.color: root.activeFocus ? palette.highlight : palette.mid

        ETBox {
            id: etbox
            focus: true
            anchors.fill: parent

            Keys.onPressed: {
                function send(code) {
                    return recvInput(code);
                }

                /* Arrow keys */
                if(event.key === Qt.Key_Left)
                    event.accepted = send(KeyCode.SLK_LEFT);
                if(event.key === Qt.Key_Right)
                    event.accepted = send(KeyCode.SLK_RIGHT);
                if(event.key === Qt.Key_Up)
                    event.accepted = send(KeyCode.SLK_UP);
                if(event.key === Qt.Key_Down)
                    event.accepted = send(KeyCode.SLK_DOWN);

                /* Special keys */
                if(event.key === Qt.Key_Return)    send(KeyCode.SLK_EXE);
                if(event.key === Qt.Key_Backspace) send(KeyCode.SLK_DEL);

                /* Operator keys */
                if(event.key === Qt.Key_Asterisk)  send(KeyCode.SLK_TIMES);
                if(event.key === Qt.Key_Slash)     send(KeyCode.SLK_DIVIDE);
                if(event.key === Qt.Key_Plus)      send(KeyCode.SLK_PLUS);
                if(event.key === Qt.Key_Minus)     send(KeyCode.SLK_MINUS);

                /* Raw input keys */
                if(event.key >= Qt.Key_A && event.key <= Qt.Key_Z)
                {
                    var shift = KeyCode.SLK_a;
                    console.log(event.modifiers, Qt.ShiftModifier);
                    if(event.modifiers & Qt.ShiftModifier) shift = KeyCode.SLK_A;
                    send(event.key - Qt.Key_A + shift);
                }
                if(event.key >= Qt.Key_0 && event.key <= Qt.Key_9)
                {
                    send(event.key - Qt.Key_0 + KeyCode.SLK_0);
                }

                /* Punctuation */
                if(event.key === Qt.Key_Equal)      send(KeyCode.SLK_COLONEQ);
                if(event.key === Qt.Key_ParenLeft)  send(KeyCode.SLK_LPAREN);
                if(event.key === Qt.Key_ParenRight) send(KeyCode.SLK_RPAREN);
                if(event.key === Qt.Key_Comma)      send(KeyCode.SLK_COMMA);
                if(event.key === Qt.Key_Period)     send(KeyCode.SLK_DOT);
                if(event.key === Qt.Key_Space)      send(KeyCode.SLK_SPACE);

                /* New keys */ // TODO : dispatch when debugged

                // cos stuff
                if (event.key === Qt.Key_F15 && (event.modifiers & Qt.ShiftModifier))
                    send(KeyCode.SLK_SQUAREROOT);

                if (event.key === Qt.Key_F10 && (event.modifiers & Qt.ShiftModifier))
                    send(KeyCode.SLK_ASIN);
                else if (event.key === Qt.Key_F10) send(KeyCode.SLK_SIN);
                if (event.key === Qt.Key_F11 && (event.modifiers & Qt.ShiftModifier))
                    send(KeyCode.SLK_ACOS);
                else if (event.key === Qt.Key_F11) send(KeyCode.SLK_COS);
                if (event.key === Qt.Key_F12 && (event.modifiers & Qt.ShiftModifier))
                    send(KeyCode.SLK_TAN);
                else if (event.key === Qt.Key_F12) send(KeyCode.SLK_TAN);
                if (event.key === Qt.Key_F13) send(KeyCode.SLK_PI);

                // pow, exponential
                if (event.key === Qt.Key_F16 && (event.modifiers & Qt.ShiftModifier))
                    send(KeyCode.SLK_LN);
                else if (event.key === Qt.Key_F16) send(KeyCode.SLK_EXP);
                if (event.key === Qt.Key_F17 && (event.modifiers & Qt.ShiftModifier))
                    send(KeyCode.SLK_LOG);
                else if (event.key === Qt.Key_F17) send(KeyCode.SLK_POWER);
                if(event.key === Qt.Key_sterling)  send(KeyCode.SLK_POWER);
                if (event.key === Qt.Key_F21) send(KeyCode.SLK_POW10);

                // Special Keys
                if(event.key === Qt.Key_F19) send(KeyCode.SLK_APPROX);
            }
        }
    }
}
