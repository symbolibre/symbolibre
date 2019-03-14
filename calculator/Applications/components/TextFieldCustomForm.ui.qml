import QtQuick 2.4
import QtQuick.Controls 2.2

Item {
    id: self
    width: 400
    height: 400
    property alias text : textField.text
    property alias textField : textField
    property string initialText : ""
    property color border : "#4E8FC1"

    property int text_prop : 2


    TextField {
        id: textField
        text: qsTr(initialText)
        font.pointSize: height/text_prop
        renderType: Text.QtRendering
        height: 395
        anchors.fill: parent

        background: Rectangle {
            border.color : self.border
            property double border_focused : self.focus ? 2 : 1
            border.width: textField.focus ? 3 : border_focused
        }
    }

}

/*##^## Designer {
    D{i:1;anchors_x:84;anchors_y:141}
}
 ##^##*/
