import QtQuick 2.12
import QtQuick.Controls 2.5

Popup {
    anchors.centerIn: Overlay.overlay
    modal: true

    // When a popup is closed, focus is redistributed to the window's content
    // item through the overlay's visual parent (the root item). But we change
    // the overlay's visual parent to be in the application (which already has
    // focus) so that redistribution ends up being a no-op.
    // To counter this, we remember which Item had the focus before opening the
    // popup and distribute it back when it is closed.
    property Item previousActiveFocus

    onAboutToShow: {
        previousActiveFocus = window.activeFocusItem;
    }
    onClosed: {
        previousActiveFocus.forceActiveFocus();
    }
}
