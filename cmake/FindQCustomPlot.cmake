include(FindPackageHandleStandardArgs)

find_package(Qt5 QUIET REQUIRED Core Gui PrintSupport Widgets)
find_path(QCustomPlot_INCLUDE_DIR qcustomplot.h)
find_library(QCustomPlot_LIBRARY qcustomplot)

find_package_handle_standard_args(QCustomPlot DEFAULT_MSG
    QCustomPlot_LIBRARY
    QCustomPlot_INCLUDE_DIR
)

set(QCustomPlot_deps
        Qt5::Core
        Qt5::Gui
        Qt5::PrintSupport
        Qt5::Widgets
)

if(QCustomPlot_FOUND)
    set(QCustomPlot_LIBRARIES
        ${QCustomPlot_LIBRARY}
        ${QCustomPlot_deps}
    )
    set(QCustomPlot_INCLUDE_DIRS
        ${QCustomPlot_INCLUDE_DIR}
    )
    set(QCustomPlot_DEFINITIONS )
endif()

if(QCustomPlot_FOUND AND NOT TARGET QCustomPlot)
    add_library(QCustomPlot UNKNOWN IMPORTED)
    set_target_properties(QCustomPlot PROPERTIES
        IMPORTED_LOCATION "${QCustomPlot_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${QCustomPlot_INCLUDE_DIR}"
    )
    set_property(TARGET QCustomPlot
        PROPERTY INTERFACE_LINK_LIBRARIES ${QCustomPlot_deps}
    )
endif()

mark_as_advanced(QCustomPlot_INCLUDE_DIR QCustomPlot_LIBRARY)
