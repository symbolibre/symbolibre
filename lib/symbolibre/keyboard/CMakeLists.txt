set(keyboard_SOURCES
    SLKey.cpp
    VirtualKeyboardContext.cpp
)
set(keyboard_HEADERS
    SLKey.hpp
    VirtualKeyboardContext.hpp
)
set(keyboard_QML_SOURCES
    Keyboard.qml
    KeyButton.qml
    ModifierButton.qml
)
set(keyboardplugin_SOURCES
    plugin.cpp
)

set(CMAKE_AUTOMOC OFF)
qt_wrap_cpp(keyboard_SOURCES ${keyboard_HEADERS})

add_library(keyboard OBJECT ${keyboard_SOURCES})
target_link_libraries(keyboard
    ${QT_DEPS}
)

set(CMAKE_AUTOMOC ON)

add_library(keyboardplugin SHARED ${keyboardplugin_SOURCES})
target_link_libraries(keyboardplugin
    Symbolibre::Symbolibre
)

install(FILES ${keyboard_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/keyboard)
install(TARGETS keyboardplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/keyboard)
install(FILES qmldir ${keyboard_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/keyboard)
