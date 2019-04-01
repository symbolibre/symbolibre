import QtQuick 2.0

Rectangle {
    width: 160
    height: 32
    color: row.activeFocus ? "lightblue" : "white"
    Text {
        width: 160
        height: 32
        id: element
        text: props.label
        font.pixelSize: 11
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
}
