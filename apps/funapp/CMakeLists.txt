cmake_minimum_required(VERSION 3.1)

project(symbolibre-funapp)

set(CMAKE_AUTOMOC ON)

if(NOT SYMBOLIBRE_BUILD)
    find_package(Symbolibre 0.2 REQUIRED)
endif()

set(funapp_QML_SOURCES
    FunApp.qml
    FunctionDefinitionDelegate.qml
    FunctionDefinitionView.qml
    FunctionMenu.qml
    GraphInterface.qml
)
set(funapp_DATA functions_default.json)

install(FILES application.json ${funapp_QML_SOURCES}
    DESTINATION ${Symbolibre_APPLETS_DIR}/funapp)
install(FILES ${funapp_DATA} DESTINATION ${Symbolibre_DATA_DIR}/funapp)
