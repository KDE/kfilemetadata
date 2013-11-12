kde4_add_plugin(kfilemetadata_mobiextractor mobiextractor.cpp)

include_directories(${QMOBIPOCKET_INCLUDE_DIR})
target_link_libraries( kfilemetadata_mobiextractor
    kfilemetadata
    ${KDE4_KIO_LIBS}
    ${QMOBIPOCKET_LIBRARIES}
)

install(
FILES kfilemetadata_mobiextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kfilemetadata_mobiextractor
DESTINATION ${PLUGIN_INSTALL_DIR})
