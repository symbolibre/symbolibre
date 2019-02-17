import QtQuick 2.8
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import org.symbolibre.mathrenderer 1.0
import SLKeyCode 1.0
import "../keyboard"

ObjectDeciderForm {
    id: objectDecider
    property int position: 0

    function recvInput(key) {
        objectDecider.contentItem.children[position].recvInput(key);
    }

    Connections {
        target: objectDecider.contentItem.children[position].id
        onExitTop: { // This is available in all editors.
            if (position != 0) {
                position = position - 1;
            }
        }
        onExitBot: {
            if (position < objectDecider.contentItem.children.length ) {
                position = position + 1;
            }
        }
    }

}
