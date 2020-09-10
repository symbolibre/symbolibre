set(controls_QML_SOURCES
    ExprField.qml
    SLStackLayout.qml
    SLStatusBar.qml
    SLTextField.qml
    SLWindow.qml
)

install(FILES qmldir ${controls_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/controls)
