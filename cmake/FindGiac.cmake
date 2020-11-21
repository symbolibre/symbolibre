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

    # a missing giac/config.h can result in segfaults
    # but some distros may not ship it
    # see e.g. https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=972827
    if(EXISTS ${Giac_INCLUDE_DIR}/giac/config.h)
        set(Giac_DEFINITIONS ${Giac_DEFINITIONS} HAVE_CONFIG_H)
    endif()
endif()

if(Giac_FOUND AND NOT TARGET Giac)
    add_library(Giac UNKNOWN IMPORTED)
    set_target_properties(Giac PROPERTIES
        IMPORTED_LOCATION "${Giac_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${Giac_INCLUDE_DIR}"
        INTERFACE_COMPILE_DEFINITIONS "${Giac_DEFINITIONS}"
    )
    set_property(TARGET Giac
        PROPERTY INTERFACE_LINK_LIBRARIES
    )
endif()

mark_as_advanced(Giac_INCLUDE_DIR Giac_LIBRARY)
