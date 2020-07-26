import QtQuick 2.12
import org.symbolibre.controls 1.0
import org.symbolibre.cas 1.0

SLWindow {
    id: window
    visible: true
    width: 320
    height: 240
    title: qsTr("symbolibre-funapp")

    MathContext {
        id: math
    }

    FunApp {
        id: app
        anchors.fill: parent
    }

    Component.onCompleted: app.forceActiveFocus();
}
