set(shell_SOURCES
    AppManager.cpp
    main.cpp
)
set(shell_QML_SOURCES
    main.qml
)

add_executable(symbolibre ${shell_SOURCES})
target_link_libraries(symbolibre
    Symbolibre::Symbolibre
    ${QT_DEPS}
    Qt::QuickControls2
    Qt::Widgets
)
install(TARGETS symbolibre DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${shell_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/shell)
