cmake_minimum_required(VERSION 3.1)

project(symbolibre-ide)

set(CMAKE_AUTOMOC ON)

find_package(Symbolibre 0.1 REQUIRED)
find_package(KF5SyntaxHighlighting REQUIRED)

set_property(GLOBAL PROPERTY CXX_STANDARD 14)

set(ide_SOURCES main.cpp languagesmodel.cpp SourceEditor.cpp)
set(ide_QML_SOURCES main.qml TextEditor.qml)

add_executable(symbolibre-ide ${ide_SOURCES})
target_link_libraries(symbolibre-ide
    Symbolibre::Symbolibre
    KF5::SyntaxHighlighting
)

install(TARGETS symbolibre-ide DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ${ide_QML_SOURCES} DESTINATION ${Symbolibre_QML_DIR}/ide)
install(DIRECTORY syntax-files DESTINATION ${Symbolibre_DATA_DIR}/ide)
