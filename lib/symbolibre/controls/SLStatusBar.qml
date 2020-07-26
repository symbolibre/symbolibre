import QtQuick 2.12
import QtQuick.Controls 2.5

Rectangle {
    x: 0
    width: parent.width
    height: 18

    color: window.palette.base
    property string label: "(Status Bar)"

    Row {
        Text {
            font.family: "DejaVu Sans"
            font.hintingPreference: Font.PreferFullHinting
            font.pixelSize: 12
            font.bold: true
            text: label

            topPadding: 1
            leftPadding: 2
        }
    }
}
