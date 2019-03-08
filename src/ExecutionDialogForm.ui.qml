import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Window 2.2
import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.2

Dialog {
    width: 320
    height: 240
    property alias execOutput: execOutput

    //property alias execOutput: execOutput
    TextArea {
        id: execOutput
        anchors.fill: parent
        font.pixelSize: 12
    }
}
