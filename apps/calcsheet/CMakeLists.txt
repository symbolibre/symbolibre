cmake_minimum_required(VERSION 3.1)

project(symbolibre-calcsheet)

set(CMAKE_AUTOMOC ON)

if(NOT SYMBOLIBRE_BUILD)
    find_package(Symbolibre 0.1 REQUIRED)
endif()

set(calcsheet_SOURCES main.cpp)
set(calcsheet_QML_SOURCES
    CalcSheet.qml
    HistoryDelegate.qml
    main.qml
)

add_executable(symbolibre-calcsheet ${calcsheet_SOURCES})
target_link_libraries(symbolibre-calcsheet
    Symbolibre::Symbolibre
)
install(TARGETS symbolibre-calcsheet DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${calcsheet_QML_SOURCES} DESTINATION ${Symbolibre_QML_DIR}/calcsheet)
