set(style_QML_SOURCES Style.qml)
install(FILES qmldir ${style_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/style)
