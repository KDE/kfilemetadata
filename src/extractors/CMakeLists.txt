if(POPPLER_QT4_FOUND)
    include_directories(${POPPLER_QT4_INCLUDE_DIR})

    kde4_add_plugin(kfilemetadata_popplerextractor popplerextractor.cpp)

    target_link_libraries(kfilemetadata_popplerextractor
        kfilemetadata
        ${KDE4_KIO_LIBS}
        ${POPPLER_QT4_LIBRARIES}
    )

    install(
    FILES kfilemetadata_popplerextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_popplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(POPPLER_QT4_FOUND)

if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    kde4_add_plugin( kfilemetadata_taglibextractor taglibextractor.cpp )

    target_link_libraries( kfilemetadata_taglibextractor
        kfilemetadata
        ${KDE4_KIO_LIBS}
        ${TAGLIB_LIBRARIES}
    )

    install(
    FILES kfilemetadata_taglibextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_taglibextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(TAGLIB_FOUND)

if(EXIV2_FOUND)
    include_directories( ${EXIV2_INCLUDE_DIR} )

    set_source_files_properties( exiv2extractor.cpp PROPERTIES
        COMPILE_FLAGS "${KDE4_ENABLE_EXCEPTIONS}"
        )

    kde4_add_plugin(kfilemetadata_exiv2extractor exiv2extractor.cpp)

    target_link_libraries(kfilemetadata_exiv2extractor
        kfilemetadata
        ${KDE4_KIO_LIBS}
        ${EXIV2_LIBRARIES}
    )

    install(
    FILES kfilemetadata_exiv2extractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_exiv2extractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(EXIV2_FOUND)

if(FFMPEG_FOUND)
    include_directories( ${FFMPEG_INCLUDE_DIRS} )

    kde4_add_plugin(kfilemetadata_ffmpegextractor ffmpegextractor.cpp)

    target_link_libraries(kfilemetadata_ffmpegextractor
        kfilemetadata
        ${KDE4_KIO_LIBS}
        ${FFMPEG_LIBRARIES}
    )

    install(
    FILES kfilemetadata_ffmpegextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_ffmpegextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(FFMPEG_FOUND)


if(EPUB_FOUND)
    include_directories( ${EPUB_INCLUDE_DIR} )

    kde4_add_plugin(kfilemetadata_epubextractor epubextractor.cpp)

    target_link_libraries(kfilemetadata_epubextractor
        kfilemetadata
        ${KDE4_KIO_LIBS}
        ${EPUB_LIBRARIES}
    )

    install(
    FILES kfilemetadata_epubextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_epubextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(EPUB_FOUND)

#
# Plain Text
#
kde4_add_plugin( kfilemetadata_plaintextextractor plaintextextractor.cpp )

target_link_libraries( kfilemetadata_plaintextextractor
    kfilemetadata
    ${KDE4_KIO_LIBS}
)

install(
FILES kfilemetadata_plaintextextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kfilemetadata_plaintextextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# ODF
#

kde4_add_plugin(kfilemetadata_odfextractor odfextractor.cpp)

target_link_libraries(kfilemetadata_odfextractor
    kfilemetadata
    ${KDE4_KIO_LIBS}
)

install(
FILES kfilemetadata_odfextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kfilemetadata_odfextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Office 2007
#

kde4_add_plugin(kfilemetadata_office2007extractor office2007extractor.cpp)

target_link_libraries(kfilemetadata_office2007extractor
    kfilemetadata
    ${KDE4_KIO_LIBS}
)

install(
FILES kfilemetadata_office2007extractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kfilemetadata_office2007extractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Office (binary formats)
#

kde4_add_plugin(kfilemetadata_officeextractor officeextractor.cpp)

target_link_libraries(kfilemetadata_officeextractor
    kfilemetadata
    ${KDE4_KIO_LIBS}
)

install(
FILES kfilemetadata_officeextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kfilemetadata_officeextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Mobipocket
#
if (QMOBIPOCKET_FOUND)
    kde4_add_plugin(kfilemetadata_mobiextractor mobiextractor.cpp)

    include_directories(${QMOBIPOCKET_INCLUDE_DIR})
    target_link_libraries(kfilemetadata_mobiextractor
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
endif(QMOBIPOCKET_FOUND)
