import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5
import org.symbolibre.keyboard 1.0

Rectangle {
    id: keyboardRoot
    color: "darkgray"

    property bool shiftPressed: shiftKey.pressed
    property bool alphaPressed: alphaKey.pressed

    GridLayout {
        anchors.centerIn: parent
        columns: 5
        rowSpacing: 1
        columnSpacing: 1

        KeyButton {
            mathKey: SLKey.F1
        }
        KeyButton {
            mathKey: SLKey.F2
        }
        KeyButton {
            mathKey: SLKey.F3
        }
        KeyButton {
            mathKey: SLKey.F4
        }
        KeyButton {
            mathKey: SLKey.F5
        }

        ModifierButton {
            id: shiftKey
            textColor: "#a0c0ff"
            key: SLKey.Shift
        }
        KeyButton {
            mathKey: SLKey.Home
        }
        KeyButton {
            mathKey: SLKey.Help
        }
        // arrow keys
        Item {
            implicitWidth: 120
            implicitHeight: 60
            Layout.rowSpan: 2
            Layout.columnSpan: 2
            KeyButton {
                width: 40
                height: 25
                mathKey: Qt.Key_Left
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            KeyButton {
                width: 40
                height: 25
                mathKey: Qt.Key_Right
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
            KeyButton {
                width: 40
                height: 25
                mathKey: Qt.Key_Up
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
            }
            KeyButton {
                width: 40
                height: 25
                mathKey: Qt.Key_Down
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        ModifierButton {
            id: alphaKey
            textColor: "#ff4040"
            key: SLKey.Alpha
        }
        KeyButton {
            mathKey: SLKey.Back
        }
        KeyButton {
            mathKey: SLKey.Undo
        }
        // arrow keys

        KeyButton {
            mathKey: SLKey.Sin
            progKey: SLKey.If
            alphaKey: Qt.Key_Colon
        }
        KeyButton {
            mathKey: SLKey.Cos
            progKey: SLKey.For
            alphaKey: Qt.Key_Semicolon
        }
        KeyButton {
            mathKey: SLKey.Tan
            progKey: SLKey.Print
            alphaKey: Qt.Key_Space
        }
        KeyButton {
            mathKey: Qt.Key_ParenLeft
        }
        KeyButton {
            mathKey: Qt.Key_ParenRight
        }

        KeyButton {
            mathKey: SLKey.Square
            progKey: SLKey.Else
            alphaKey: Qt.Key_A
        }
        KeyButton {
            mathKey: SLKey.LogE
            progKey: SLKey.While
            alphaKey: Qt.Key_B
        }
        KeyButton {
            mathKey: SLKey.Log10
            progKey: SLKey.Input
            alphaKey: Qt.Key_C
        }
        KeyButton {
            mathKey: SLKey.Pi
            progKey: Qt.Key_BracketLeft
            alphaKey: Qt.Key_D
        }
        KeyButton {
            progKey: Qt.Key_BracketRight
            alphaKey: Qt.Key_E
        }

        KeyButton {
            mathKey: SLKey.Sqrt
            alphaKey: Qt.Key_F
        }
        KeyButton {
            mathKey: SLKey.Exp
            alphaKey: Qt.Key_G
        }
        KeyButton {
            mathKey: SLKey.Pow
            alphaKey: Qt.Key_H
        }
        KeyButton {
            mathKey: SLKey.Imag
            alphaKey: Qt.Key_I
        }
        KeyButton {
            mathKey: Qt.Key_Equal
            alphaKey: Qt.Key_J
        }

        KeyButton {
            mathKey: Qt.Key_7
            alphaKey: Qt.Key_K
        }
        KeyButton {
            mathKey: Qt.Key_8
            alphaKey: Qt.Key_L
        }
        KeyButton {
            mathKey: Qt.Key_9
            alphaKey: Qt.Key_M
        }
        KeyButton {
            mathKey: Qt.Key_Asterisk
            alphaKey: Qt.Key_N
        }
        KeyButton {
            mathKey: Qt.Key_Slash
            alphaKey: Qt.Key_O
        }

        KeyButton {
            mathKey: Qt.Key_4
            alphaKey: Qt.Key_P
        }
        KeyButton {
            mathKey: Qt.Key_5
            alphaKey: Qt.Key_Q
        }
        KeyButton {
            mathKey: Qt.Key_6
            alphaKey: Qt.Key_R
        }
        KeyButton {
            mathKey: Qt.Key_Plus
            alphaKey: Qt.Key_S
        }
        KeyButton {
            mathKey: Qt.Key_Minus
            alphaKey: Qt.Key_T
        }

        KeyButton {
            mathKey: Qt.Key_1
            alphaKey: Qt.Key_U
        }
        KeyButton {
            mathKey: Qt.Key_2
            alphaKey: Qt.Key_V
        }
        KeyButton {
            mathKey: Qt.Key_3
            alphaKey: Qt.Key_W
        }
        KeyButton {
            mathKey: Qt.Key_X
            alphaKey: Qt.Key_X
        }
        KeyButton {
            mathKey: Qt.Key_Percent
            alphaKey: Qt.Key_Y
        }

        KeyButton {
            mathKey: Qt.Key_0
            alphaKey: Qt.Key_Z
        }
        KeyButton {
            mathKey: Qt.Key_Period
        }
        KeyButton {
            mathKey: Qt.Key_Comma
        }
        KeyButton {
            mathKey: SLKey.Delete
        }
        KeyButton {
            mathKey: SLKey.Enter
        }
    }
}
