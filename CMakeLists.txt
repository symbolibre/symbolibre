cmake_minimum_required(VERSION 3.1)

project(Symbolibre
    VERSION 0.2
    DESCRIPTION "Calculator software"
    HOMEPAGE_URL "https://symbolibre.org"
)

include(CTest)
include(GNUInstallDirs)
include(FeatureSummary)
set(CMAKE_AUTOMOC ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(Qt5 5.12 REQUIRED Core Gui Qml Quick QuickControls2 Widgets
    OPTIONAL_COMPONENTS LinguistTools Test)
find_package(QCustomPlot REQUIRED)
find_package(Giac REQUIRED)

if(BUILD_TESTING AND NOT(${Qt5Test_FOUND}))
    message(SEND_ERROR "Tests enabled but Qt5Test not found")
endif()

set(DEV_BUILD OFF CACHE BOOL "Developer build")
set(ENABLE_APPS ON CACHE BOOL "Build Symbolibre applications")
set(USE_CUSTOM_QML_PATH ON CACHE BOOL
    "Install QML modules in a Symbolibre-specific location")
set(SET_RPATH ${DEV_BUILD} CACHE BOOL
    "Set the RPATH of the applications to the install path of libSymbolibre")
set(ENABLE_DOC OFF CACHE BOOL "Build HTML documentation")

if(ENABLE_DOC)
    find_program(SphinxBuild NAMES sphinx-build REQUIRED)
endif()

# signal the applications that they do not need to find_package(Symbolibre)
set(SYMBOLIBRE_BUILD True)

set(SL_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/symbolibre)
set(SL_DATA_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/symbolibre)
set(SL_THEME_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/symbolibre/theme)
set(SL_APPLETS_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/symbolibre/apps)

file(RELATIVE_PATH SL_PREFIX_FROM_LIB_DIR
    ${CMAKE_INSTALL_FULL_LIBDIR} ${CMAKE_INSTALL_PREFIX})

if(USE_CUSTOM_QML_PATH)
    set(SL_QML_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/symbolibre/qml)
    get_filename_component(SL_FULL_QML_INSTALL_DIR ${SL_QML_INSTALL_DIR}
        ABSOLUTE BASE_DIR ${CMAKE_INSTALL_PREFIX})

    # *only* used by IDEs
    set(QML_IMPORT_PATH ${SL_FULL_QML_INSTALL_DIR} CACHE STRING "QML search paths for IDEs")

else()
    if(QMAKE_QT_EXECUTABLE) # set by Qt Creator
        set(QMAKE_COMMAND ${QMAKE_QT_EXECUTABLE} CACHE STRING "qmake executable")
    else()
        # FIXME we should use Qt5::qmake here, but it doesn't seem to work
        # with execute_process
        set(QMAKE_COMMAND qmake CACHE STRING "qmake executable")
    endif()
    execute_process(COMMAND "${QMAKE_COMMAND}" -query QT_INSTALL_QML
        RESULT_VARIABLE QML_INSTALL_DIR_QMAKE_STATUS
        OUTPUT_VARIABLE QML_INSTALL_DIR
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    if(NOT(QML_INSTALL_DIR_QMAKE_STATUS EQUAL 0) OR NOT(QML_INSTALL_DIR))
        message(FATAL_ERROR "Unable to query qmake")
    endif()

    set(SL_FULL_QML_INSTALL_DIR "${QML_INSTALL_DIR}")
    if(NOT(SL_FULL_QML_INSTALL_DIR MATCHES "^${CMAKE_INSTALL_PREFIX}"))
        message(WARNING "QML modules will be installed outside the install prefix.")
    endif()
    message(STATUS "QML modules will be installed to ${SL_FULL_QML_INSTALL_DIR}")
    set(SL_QML_INSTALL_DIR ${SL_FULL_QML_INSTALL_DIR})
endif()

# for config.hpp
set(SL_CUSTOM_QML_DIR USE_CUSTOM_QML_PATH)
if(UNIX)
    set(SL_TARGET_UNIX True)
elseif(WIN32)
    set(SL_TARGET_WIN32 True)
endif()

if(SET_RPATH)
    set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})
endif()

set(QT_DEPS Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick)

set_property(GLOBAL PROPERTY CXX_STANDARD 14)

if(DEV_BUILD)
    if(MSVC)
        add_compile_options(/W4)
    else()
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()
endif()

# set variables just like SymbolibreConfig
set(Symbolibre_QML_DIR ${SL_QML_INSTALL_DIR})
set(Symbolibre_DATA_DIR ${SL_DATA_INSTALL_DIR})
set(Symbolibre_APPLETS_DIR ${SL_APPLETS_INSTALL_DIR})

feature_summary(WHAT ALL)

include_directories(lib)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)

add_subdirectory(theme)
add_subdirectory(lib)
add_subdirectory(shell)

add_subdirectory(apps)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_subdirectory(cmake)

install(DIRECTORY icons DESTINATION ${SL_DATA_INSTALL_DIR})

if(Qt5LinguistTools_FOUND)
    # FIXME this should be an add_custom_command with the right deps
    add_custom_target(translation
        COMMAND Qt5::lupdate lib/ apps/ shell/
            -ts ${CMAKE_CURRENT_BINARY_DIR}/symbolibre.ts
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(translations)
endif()

if(ENABLE_DOC)
    add_subdirectory(doc)
endif()
