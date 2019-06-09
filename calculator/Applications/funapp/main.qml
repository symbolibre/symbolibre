import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2

ApplicationWindow {
    visible: true
    width: 320
    height: 220
    font.pixelSize: 13
    title: qsTr("Quick Calculator")

    MainForm {
        anchors.fill: parent
    }
}
