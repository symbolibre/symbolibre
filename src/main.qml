import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0

ApplicationWindow {
    id: window
    width: 320
    height: 240
    visible: true

    Grid {
        id: grid
        anchors.fill: parent
    }

    TextArea {
        id: textArea
        text: ""
        anchors.topMargin: 39
        anchors.fill: parent
        wrapMode: Text.WrapAnywhere
        readOnly: false
    }

    ToolButton {
        id: toolButton
        x: 0
        y: 0
        text: "Ouvrir"
    }


}

/*##^## Designer {
    D{i:20;anchors_height:125;anchors_width:175;anchors_x:46;anchors_y:75}D{i:29;anchors_x:40;anchors_y:53}
}
 ##^##*/
