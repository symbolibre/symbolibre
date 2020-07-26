import QtQuick 2.12

Rectangle {
    property alias text: t.text

    width: parent.width / 5
    height: parent.height

    Text {
        id: t
        anchors.fill: parent

        font.family: window.font.family
        font.hintingPreference: window.font.hintingPreference
        font.pixelSize: 10

        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        lineHeight: 0.8
        color: "white"

        /* Alternative for bottom alignment:
        verticalAlignment: Text.AlignBottom
        bottomPadding: 1 */
    }
}
