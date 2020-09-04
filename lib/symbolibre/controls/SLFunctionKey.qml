import QtQuick 2.12
import org.symbolibre.style 1.0

Rectangle {
    id: key
    property alias text: t.text
    property var model

    width: parent.width / 5
    height: parent.height
    color: model.color || window.palette.mid

    Text {
        id: t
        anchors.fill: parent
        font.pixelSize: SLStyle.smallPixelSize

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        lineHeight: 0.8
        color: "white"
        text: model.text || ""

        /* Alternative for bottom alignment:
        verticalAlignment: Text.AlignBottom
        bottomPadding: 1 */
    }

    MouseArea {
        anchors.fill: parent
        onPressed: key.parent.activated(model.action);
    }
}
