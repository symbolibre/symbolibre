import QtQuick 2.0
import QtQuick.Layouts 1.3

/* This component is a Column Layout that distributes focus (ie keyboard inputs) to its sub-components */
/* It suppose that it cannot gain focus throught a click */


ColumnLayout {
        id: layout
        property int focusedChild: 0

        onFocusChanged: { // Very important, so it gives focus to the first item when it receives it
            if (focus == true) {
                children[focusedChild].forceActiveFocus()
            }
        }

        Keys.onDownPressed: { // Down pressed: give focus to next item
            var accept = 0
            if (focusedChild < children.length - 1) {
                focusedChild = focusedChild + 1
                accept = 1
            }
            if (accept == 0) {
                event.accepted = false
            }
            else {
                event.accepted = true
            }
        }
         Keys.onUpPressed: { // Up pressed: give focus to previous item
             var accept = 0
             if (focusedChild > 0) {
                 focusedChild = focusedChild - 1
                 accept = 1
             }
             if (accept == 0) {
                 event.accepted = false
             }
             else {
                 event.accepted = true
             }
         }

         onFocusedChildChanged: {
             children[focusedChild].forceActiveFocus()
         }
}
