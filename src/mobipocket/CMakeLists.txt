kde4_add_plugin( nepomukmobiextractor mobiextractor.cpp )

include_directories(${QMOBIPOCKET_INCLUDE_DIR})
target_link_libraries( nepomukmobiextractor
    nepomukcore
    nepomukextractor
    ${KDE4_KIO_LIBS}
    ${QMOBIPOCKET_LIBRARIES}
)

install(
FILES nepomukmobiextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS nepomukmobiextractor
DESTINATION ${PLUGIN_INSTALL_DIR})
