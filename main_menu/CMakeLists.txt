set(menu_SOURCES
    AppLauncher.cpp
    main.cpp
)
set(menu_QML_SOURCES
    main.qml
    Menu.qml
)

add_executable(symbolibre ${menu_SOURCES})
target_link_libraries(symbolibre
    Symbolibre::Symbolibre
    ${QT_DEPS}
    Qt::QuickControls2
    Qt::Widgets
)
install(TARGETS symbolibre DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${menu_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/menu)
install(DIRECTORY applications DESTINATION ${SL_DATA_INSTALL_DIR})
install(DIRECTORY icons DESTINATION ${SL_DATA_INSTALL_DIR})
