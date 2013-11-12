kde4_add_plugin(kmetadata_mobiextractor mobiextractor.cpp)

include_directories(${QMOBIPOCKET_INCLUDE_DIR})
target_link_libraries( kmetadata_mobiextractor
    kmetadataextractor
    ${KDE4_KIO_LIBS}
    ${QMOBIPOCKET_LIBRARIES}
)

install(
FILES kmetadata_mobiextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kmetadata_mobiextractor
DESTINATION ${PLUGIN_INSTALL_DIR})
