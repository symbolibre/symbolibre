set(TRANSLATIONS fr)

foreach(LANG ${TRANSLATIONS})
    set(TSFILE ${CMAKE_CURRENT_SOURCE_DIR}/symbolibre.${LANG}.ts)
    set(QMFILE symbolibre.${LANG}.qm)
    add_custom_command(OUTPUT ${QMFILE}
        COMMAND Qt5::lrelease ${TSFILE} -qm ${QMFILE}
        DEPENDS ${TSFILE})
    add_custom_target(${LANG} ALL DEPENDS ${QMFILE})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QMFILE}
        DESTINATION ${SL_DATA_INSTALL_DIR}/translations)

    # Custom target without output is always out-of-date
    add_custom_target(translation-${LANG}
        COMMAND Qt5::lconvert ${CMAKE_BINARY_DIR}/symbolibre.ts ${TSFILE}
            -target-language ${LANG} -o ${TSFILE}
        DEPENDS ${CMAKE_BINARY_DIR}/symbolibre.ts)
endforeach()
