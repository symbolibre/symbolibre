import QtQuick 2.12
import org.symbolibre.controls 1.0

SLWindow {
    id: window

    visible: true
    width: 320
    height: 220
    title: qsTr("Quick Calculator")

    FunApp {
        anchors.fill: parent
    }
}
