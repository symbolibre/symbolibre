import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import Qt.labs.platform 1.0

import DocumentHandler 1.0

ApplicationWindow {
    id: mainwindow
    visible: true
    width: 320
    height: 240

    TextEditor {
        id: editor
        window: mainwindow
        Component.onCompleted: newOpenSelection.forceActiveFocus()
    }

    title: editor.fileName + qsTr(" - Symbolibre IDE")
}
