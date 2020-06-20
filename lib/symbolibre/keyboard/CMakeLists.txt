set(keyboard_SOURCES
    keycode.cpp
    VirtualKeyboardContext.cpp
)
set(keyboard_HEADERS
    keycode.hpp
    VirtualKeyboardContext.hpp
)
set(keyboard_QML_SOURCES
    CalcButtonForm.ui.qml
    CalcButton.qml
    SLKeyBoardForm.ui.qml
    SLKeyBoard.qml
    SpecialButtonForm.ui.qml
    SpecialButton.qml
)
set(keyboardplugin_SOURCES
    plugin.cpp
)

add_library(keyboard OBJECT ${keyboard_SOURCES})
target_link_libraries(keyboard
    ${QT_DEPS}
)

add_library(keyboardplugin SHARED ${keyboardplugin_SOURCES})
target_link_libraries(keyboardplugin
    Symbolibre::Symbolibre
)

install(FILES ${keyboard_HEADERS} DESTINATION ${SL_INCLUDE_INSTALL_DIR}/keyboard)
install(TARGETS keyboardplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/keyboard)
install(FILES qmldir ${keyboard_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/keyboard)
