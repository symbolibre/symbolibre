include(FindPackageHandleStandardArgs)

find_path(Giac_INCLUDE_DIR giac/giac.h)
find_library(Giac_LIBRARY giac)

find_package_handle_standard_args(Giac DEFAULT_MSG
    Giac_LIBRARY
    Giac_INCLUDE_DIR
)

if(Giac_FOUND)
    set(Giac_LIBRARIES
        ${Giac_LIBRARY}
    )
    set(Giac_INCLUDE_DIRS
        ${Giac_INCLUDE_DIR}
    )
    set(Giac_DEFINITIONS )
endif()

if(Giac_FOUND AND NOT TARGET Giac)
    add_library(Giac UNKNOWN IMPORTED)
    set_target_properties(Giac PROPERTIES
        IMPORTED_LOCATION "${Giac_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Giac_INCLUDE_DIR}"
    )
    set_property(TARGET Giac
        PROPERTY INTERFACE_LINK_LIBRARIES
    )
endif()

mark_as_advanced(Giac_INCLUDE_DIR Giac_LIBRARY)
