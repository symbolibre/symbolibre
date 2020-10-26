cmake_minimum_required(VERSION 3.1)

project(symbolibre-ide)

set(CMAKE_AUTOMOC ON)

if(NOT SYMBOLIBRE_BUILD)
    find_package(Symbolibre 0.1 REQUIRED)
endif()

find_package(KF5SyntaxHighlighting REQUIRED)
find_package(qtermwidget5 REQUIRED)

set_property(GLOBAL PROPERTY CXX_STANDARD 14)

set(ideplugin_SOURCES languagesmodel.cpp plugin.cpp SourceDocument.cpp)
set(ide_QML_SOURCES IDE.qml Editor.qml)

add_library(ideplugin SHARED ${ideplugin_SOURCES})
target_link_libraries(ideplugin
    Symbolibre::Symbolibre
    KF5::SyntaxHighlighting
    qtermwidget5
)

install(TARGETS ideplugin
    DESTINATION ${Symbolibre_QML_DIR}/org/symbolibre/editor)
install(FILES qmldir
    DESTINATION ${Symbolibre_QML_DIR}/org/symbolibre/editor)
install(FILES application.json ${ide_QML_SOURCES}
    DESTINATION ${Symbolibre_APPLETS_DIR}/ide)

install(FILES languages.json DESTINATION ${Symbolibre_DATA_DIR}/ide)
install(DIRECTORY syntax-files DESTINATION ${Symbolibre_DATA_DIR}/ide)
install(DIRECTORY catalog DESTINATION ${Symbolibre_DATA_DIR})
install(DIRECTORY programs DESTINATION ${Symbolibre_DATA_DIR}/ide)
