import QtQuick 2.12
import org.symbolibre.keyboard 1.0
import org.symbolibre.controls 1.0

SLWindow {
    id: window
    visible: true
    width: 320
    height: 220 + (keyboard.active ? 240 : 0)
    title: qsTr("Quick Calculator")

    CalcSheet {
        anchors.fill: parent
    }
}
