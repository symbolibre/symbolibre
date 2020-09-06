import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    property Dialog errorDialog;

    palette.window: "white"
    palette.windowText: "black"
    palette.base: "white"
    palette.alternateBase: "lightgray"
    palette.text: "black"
    palette.buttonText: "white"
    palette.brightText: "white"
    palette.highlight: "#4080c0"
    palette.highlightedText: "white"

    palette.button: "#2b79b5"
    palette.light: "#0a67af"
    palette.midlight: "#4e8fc1"
    palette.dark: "#053d68"
    palette.mid: "#074e84"

    Dialog {
        id: errorDialog
        anchors.centerIn: Overlay.overlay
        focus: true
        modal: true
        contentItem: Text {
            id: errorText
        }
        standardButtons: Dialog.Ok
    }

    function showError(text, title) {
        errorDialog.title = title || qsTr("Error");
        errorText.text = text;
        errorDialog.open();
    }
}
