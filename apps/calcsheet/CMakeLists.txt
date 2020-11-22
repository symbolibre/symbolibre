cmake_minimum_required(VERSION 3.1)

project(symbolibre-calcsheet)

set(CMAKE_AUTOMOC ON)

if(NOT SYMBOLIBRE_BUILD)
    find_package(Symbolibre 0.2 REQUIRED)
endif()

set(calcsheet_QML_SOURCES
    CalcSheet.qml
    HistoryDelegate.qml
)

install(FILES application.json ${calcsheet_QML_SOURCES}
    DESTINATION ${Symbolibre_APPLETS_DIR}/calcsheet)
