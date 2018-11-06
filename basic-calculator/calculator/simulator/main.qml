import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import io.qt.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0

Window {
    visible: true
    width: 320
    height: 240*2
    title: qsTr("Quick Calculator")

    MainForm {
        anchors.fill: parent
        keyboard.onActivated: {
            formula.recvInput(key);
            formula.update();
        }
    }
}
