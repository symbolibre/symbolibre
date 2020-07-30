set(calcsheet_SOURCES main.cpp)
set(calcsheet_QML_SOURCES
    CalcSheet.qml
    HistoryDelegate.qml
    main.qml
)

add_executable(symbolibre-calcsheet ${calcsheet_SOURCES})
target_link_libraries(symbolibre-calcsheet
    Symbolibre::Symbolibre
    ${QT_DEPS}
)
install(TARGETS symbolibre-calcsheet DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${calcsheet_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/calcsheet)
