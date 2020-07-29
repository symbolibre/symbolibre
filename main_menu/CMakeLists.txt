set(menu_SOURCES
    applauncher.cpp
    main.cpp
)
set(menu_QML_SOURCES
    main.qml
)
configure_file(qtquickcontrols2.conf.in qtquickcontrols2.conf)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/qqc2conf.qrc"
    "<RCC><qresource prefix=\"/\">
    <file>qtquickcontrols2.conf</file>
    </qresource></RCC>"
)
qt5_add_resources(menu_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/qqc2conf.qrc"
)

add_executable(symbolibre-menu ${menu_SOURCES})
target_link_libraries(symbolibre-menu
    Symbolibre::Symbolibre
    ${QT_DEPS}
)
install(TARGETS symbolibre-menu DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${menu_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/menu)
install(DIRECTORY applications DESTINATION ${SL_DATA_INSTALL_DIR})
install(DIRECTORY icons DESTINATION ${SL_DATA_INSTALL_DIR})
