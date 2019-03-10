import QtQuick 2.7
import QtQuick.Controls 2.0
import CustomPlot 1.0
import SLKeyCode 1.0

Rectangle {
    width: 320
    height: 510
    property alias plot: plot
    property alias entree: entree
    property alias keyboard: keyboard

    Rectangle {
        id: graphrenderer
        height: 240
        width: 320
        x: 0
        y: 0
        color: "white"

        CustomPlotItem {
            id: plot
            height: 240
            width: 320
            x: 0
            y: 0
        }
    }

    Rectangle {
        id: dap
        height: 30
        width: 320
        x: 0
        y: 240
        color: "white"

        TextField {
            id: entree
            height: 30
            width: 320
        }

    }

    Rectangle {
        id: pad
        height: 240
        width: 320
        x: 0
        y: 270
        color: "black"

        SLKeyBoard {
            id: keyboard
        }
    }
}
