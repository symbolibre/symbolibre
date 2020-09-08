set(TRANSLATIONS )

foreach(LANG ${TRANSLATIONS})
    set(TSFILE ${CMAKE_CURRENT_SOURCE_DIR}/symbolibre.${LANG}.ts)
    set(QMFILE symbolibre.${LANG}.qm)
    add_custom_command(OUTPUT ${QMFILE}
        COMMAND $<TARGET_FILE:Qt5::lrelease> ${TSFILE} -qm ${QMFILE}
        DEPENDS ${TSFILE})
    add_custom_target(${LANG} ALL DEPENDS ${QMFILE})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${QMFILE}
        DESTINATION ${SL_DATA_INSTALL_DIR}/translations)
endforeach()
