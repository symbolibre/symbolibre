import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import SLKeyCode 1.0
import "../keyboard"

Window {
    visible: true
    width: 320
    height: 220+240
    title: qsTr("Quick Calculator")

    MainForm {
        anchors.fill: parent
        keyboard.onActivated: {
            formula.recvInput(key);
            formula.update();
        }
    }
}
