import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"

MathRenderer {

    Keys.onPressed: {
        function send(code) {
            formula.recvInput(code);
        }

        /* Arrow keys */
        if(event.key == Qt.Key_Left)      send(KeyCode.SLK_LEFT);
        if(event.key == Qt.Key_Right)     send(KeyCode.SLK_RIGHT);
        if(event.key == Qt.Key_Up)        send(KeyCode.SLK_UP);
        if(event.key == Qt.Key_Down)      send(KeyCode.SLK_DOWN);

        /* Special keys */
        if(event.key == Qt.Key_Return)    send(KeyCode.SLK_EXE);
        if(event.key == Qt.Key_Backspace) send(KeyCode.SLK_DEL);

        /* Operator keys */
        if(event.key == Qt.Key_Asterisk)  send(KeyCode.SLK_TIMES);
        if(event.key == Qt.Key_Slash)     send(KeyCode.SLK_DIVIDE);
        if(event.key == Qt.Key_Plus)      send(KeyCode.SLK_PLUS);
        if(event.key == Qt.Key_Minus)     send(KeyCode.SLK_MINUS);

        /* Raw input keys */
        if(event.key >= Qt.Key_A && event.key <= Qt.Key_Z)
        {
            var shift = KeyCode.SLK_a;
            if(event.mod & Qt.ShiftModifier) shift = KeyCode.SLK_A;
            send(event.key - Qt.Key_A + shift);
        }
        if(event.key >= Qt.Key_0 && event.key <= Qt.Key_9)
        {
            send(event.key - Qt.Key_0 + KeyCode.SLK_0);
        }

        /* Punctuation */
        if(event.key == Qt.Key_Equal)      send(KeyCode.SLK_COLONEQ);
        if(event.key == Qt.Key_ParenLeft)  send(KeyCode.SLK_LPAREN);
        if(event.key == Qt.Key_ParenRight) send(KeyCode.SLK_RPAREN);

        /* Functions */
        if(event.key == Qt.Key_F15 && (event.mod & Qt.ShiftModifer)) send(KeyCode.SLK_SQUAREROOT);
        if(event.key == Qt.Key_F19) send(KeyCode.SLK_APPROX);

        formula.update();
    }
}
