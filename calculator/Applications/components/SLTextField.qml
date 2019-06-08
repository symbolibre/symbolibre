import QtQuick 2.4
import QtQuick.Controls 2.2

TextField {
    property bool editFocus: false
    property color borderColor : "#4E8FC1"

    focus: true

    readOnly: !editFocus

    font.pixelSize: 13
    renderType: Text.QtRendering

    background: Rectangle {
        border.color : parent.borderColor
        border.width: parent.editFocus ? 3 : parent.activeFocus ? 2 : 1
    }

    Keys.onReturnPressed: {
        editFocus = !editFocus
    }

    onActiveFocusChanged: if (!activeFocus) editFocus = false
}
