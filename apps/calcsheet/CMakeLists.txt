set(calcsheet_SOURCES main.cpp)
set(calcsheet_QML_SOURCES
    CalcSheet.qml
    HistoryDelegate.qml
    main.qml
)
configure_file(qtquickcontrols2.conf.in qtquickcontrols2.conf)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/qqc2conf.qrc"
    "<RCC><qresource prefix=\"/\">
    <file>qtquickcontrols2.conf</file>
    </qresource></RCC>"
)
qt5_add_resources(calcsheet_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/qqc2conf.qrc")

add_executable(symbolibre-calcsheet ${calcsheet_SOURCES})
target_link_libraries(symbolibre-calcsheet
    Symbolibre::Symbolibre
    ${QT_DEPS}
)
install(TARGETS symbolibre-calcsheet DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${calcsheet_QML_SOURCES} DESTINATION ${SL_QML_INSTALL_DIR}/calcsheet)
