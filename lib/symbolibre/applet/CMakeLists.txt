set(applet_QML_SOURCES
    SLFunctionBar.qml
    SLFunctionKey.qml
    SLStandardApplet.qml
)

set(applet_SOURCES
    FunctionBarAttached.cpp
    AppletHelper.cpp
)
set(applet_HEADERS
    FunctionBarAttached.hpp
    AppletHelper.hpp
)

set(appletplugin_SOURCES
    plugin.cpp
)

set(CMAKE_AUTOMOC OFF)
qt5_wrap_cpp(applet_SOURCES ${applet_HEADERS})

add_library(applet OBJECT ${applet_SOURCES})
target_link_libraries(applet ${QT_DEPS})

set(CMAKE_AUTOMOC ON)
add_library(appletplugin SHARED ${appletplugin_SOURCES})
target_link_libraries(appletplugin Symbolibre::Symbolibre)

install(FILES qmldir ${applet_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/applet)
install(TARGETS appletplugin
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/applet)
