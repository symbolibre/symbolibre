import QtQuick 2.4

TextFieldCustomForm {
    Keys.onReturnPressed: {
        if (focus == true) {
            textField.forceActiveFocus()
        }
        else {
            forceActiveFocus()
        }
    }
}
