set(funapp_SOURCES main.cpp)
set(funapp_QML_SOURCES
    FunctionDefinitionDelegate.qml
    FunctionDefinitionView.qml
    FunctionMenu.qml
    GraphInterface.qml
    main.qml
)
configure_file(qtquickcontrols2.conf.in qtquickcontrols2.conf)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/qqc2conf.qrc"
    "<RCC><qresource prefix=\"/\">
    <file>qtquickcontrols2.conf</file>
    </qresource></RCC>"
)
qt5_add_resources(funapp_SOURCES
    funapp.qrc
    "${CMAKE_CURRENT_BINARY_DIR}/qqc2conf.qrc"
)

add_executable(funapp ${funapp_SOURCES})
target_link_libraries(funapp
    ${slmath_LIBRARY}
    ${QT_DEPS}
    Qt::Widgets
)
install(TARGETS funapp DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${funapp_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/funapp)
