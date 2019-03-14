import QtQuick 2.0

Item {
    property int columns : 2
    property int focusedChild: 0

    property var nameList : {
        var nameList = []
        var i
        for (i = 0; i < children.length; i = i+2) {
            nameList.push(children[i].text)
        }
        return nameList
    }

    property var exprList : {
        var exprList = []
        var i
        for (i = 1; i < children.length; i = i+2) {
            exprList.push(children[i].text)
        }
        return exprList
    }

    onFocusChanged: { // Very important, so it gives focus to the first item when it receives it
        if (focus == true) {
            focusedChild = 0
            children[focusedChild].forceActiveFocus()
        }
    }

    Keys.onRightPressed: { // Down pressed: give focus to next item
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
     Keys.onLeftPressed: { // Up pressed: give focus to previous item
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

     Keys.onDownPressed: { // Down pressed: give focus to next item
         var accept = 0
         if (focusedChild < children.length - columns) {
             focusedChild = focusedChild + columns
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
          if (focusedChild >= columns) {
              focusedChild = focusedChild - columns
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
