import QtQuick 2.8
import QtQuick.Controls 2.2
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"
Rectangle {
    width: 320
    height: 240 * 2
    property alias formula: formula
    property alias keyboard: keyboard

    Rectangle {
        id: renderer
        height: 240
        width: 320
        x: 0
        y: 0
        color: "white"

        MathRenderer {
            id: formula
            height: 240
            width: 320
            x: 0
            y: 0
        }
    }

    Rectangle {
        id: pad
        height: 240
        width: 320
        x: 0
        y: 240
        color: "black"

        SLKeyBoard {
            id: keyboard
        }
    }
}
