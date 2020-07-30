set(funapp_SOURCES main.cpp)
set(funapp_QML_SOURCES
    FunApp.qml
    FunctionDefinitionDelegate.qml
    FunctionDefinitionView.qml
    FunctionMenu.qml
    GraphInterface.qml
    main.qml
)

add_executable(symbolibre-funapp ${funapp_SOURCES})
target_link_libraries(symbolibre-funapp
    Symbolibre::Symbolibre
    ${QT_DEPS}
    Qt::QuickControls2
    Qt::Widgets
)
install(TARGETS symbolibre-funapp DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${funapp_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/funapp)
