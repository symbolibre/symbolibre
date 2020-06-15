set(controls_QML_SOURCES
    SLCheckBox.qml
    SLStackLayout.qml
    SLTabBar.qml
    SLTextField.qml
    SLWindow.qml
)

install(FILES qmldir ${controls_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/controls)
