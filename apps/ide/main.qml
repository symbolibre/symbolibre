import QtQuick 2.12
import QtQuick.Controls 2.5

ApplicationWindow {
    id: mainwindow
    visible: true
    width: 320
    height: 240

    TextEditor {
        id: editor
        anchors.fill: parent
    }

    title: editor.fileName + qsTr(" - Symbolibre IDE")
}
