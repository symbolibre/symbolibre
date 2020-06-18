import QtQuick 2.12
import QtQuick.Controls 2.2

TextField {
    property bool editFocus: false
    property color borderColor : activeFocus ? palette.highlight : palette.windowText

    focus: true
    readOnly: !editFocus

    background: Rectangle {
        border.color : parent.editFocus || parent.activeFocus ? parent.borderColor : "#a0a0a0"
        border.width: parent.editFocus ? 3 : parent.activeFocus ? 2 : 1
    }

    TapHandler {
        acceptedButtons: Qt.AllButtons
        onTapped: {
            forceActiveFocus();
            editFocus = true;
        }
    }

    Keys.onReturnPressed: {
        editFocus = !editFocus
    }

    Keys.onLeftPressed: {
        if (readOnly) {
            cursorPosition = 0;
        }
        event.accepted = false;
    }

    Keys.onRightPressed: {
        if (readOnly) {
            cursorPosition = text.length;
        }
        event.accepted = false;
    }

    onActiveFocusChanged: if (!activeFocus) editFocus = false
}
