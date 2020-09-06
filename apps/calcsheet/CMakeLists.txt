cmake_minimum_required(VERSION 3.1)

project(symbolibre-calcsheet)

set(CMAKE_AUTOMOC ON)

if(NOT SYMBOLIBRE_BUILD)
    find_package(Symbolibre 0.1 REQUIRED)
endif()

set(calcsheet_QML_SOURCES
    CalcSheet.qml
    HistoryDelegate.qml
)

install(FILES ${calcsheet_QML_SOURCES} DESTINATION ${Symbolibre_QML_DIR}/calcsheet)
