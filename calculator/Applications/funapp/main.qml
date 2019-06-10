import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

ApplicationWindow {
    visible: true
    width: 320
    height: 220
    font.pixelSize: 13
    title: qsTr("Quick Calculator")

    palette.window: "white"
    palette.windowText: "black"
    palette.base: "white"
    palette.alternateBase: "lightgray"
    palette.text: "black"
    palette.buttonText: "white"
    palette.brightText: "white"
    palette.highlight: "darkblue"
    palette.highlightedText: "white"

    palette.button: "#2b79b5"
    palette.light: "#0a67af"
    palette.midlight: "#4e8fc1"
    palette.dark: "#053d68"
    palette.mid: "#074e84"

    MainForm {
        anchors.fill: parent
    }
}
