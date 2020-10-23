set(catalog_QML_SOURCES
    Catalog.qml
    CatalogModel.qml
    CatalogPage.qml
    CatalogPopup.qml
)

install(FILES qmldir ${catalog_QML_SOURCES}
    DESTINATION ${SL_QML_INSTALL_DIR}/org/symbolibre/catalog)
install(DIRECTORY data/
    DESTINATION ${SL_DATA_INSTALL_DIR}/catalog)
