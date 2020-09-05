cmake_minimum_required(VERSION 3.1)

project(symbolibre-funapp)

set(CMAKE_AUTOMOC ON)

if(NOT SYMBOLIBRE_BUILD)
    find_package(Symbolibre 0.1 REQUIRED)
endif()

set(funapp_SOURCES main.cpp)
set(funapp_QML_SOURCES
    FunApp.qml
    FunctionDefinitionDelegate.qml
    FunctionDefinitionView.qml
    FunctionMenu.qml
    GraphInterface.qml
    main.qml
)
set(funapp_DATA functions_default.json)

add_executable(symbolibre-funapp ${funapp_SOURCES})
target_link_libraries(symbolibre-funapp
    Symbolibre::Symbolibre
)

install(TARGETS symbolibre-funapp DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${funapp_QML_SOURCES} DESTINATION ${Symbolibre_QML_DIR}/funapp)
install(FILES ${funapp_DATA} DESTINATION ${Symbolibre_DATA_DIR}/funapp)
