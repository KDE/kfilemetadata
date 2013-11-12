include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
)

kde4_add_library(kmetadataextractor SHARED extractorplugin.cpp)

target_link_libraries(kmetadataextractor
  ${QT_QTCORE_LIBRARY}
  ${KDE4_KDECORE_LIBS}
)

install(TARGETS kmetadataextractor EXPORT KdeLibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

# -----------------------------
# Extractors
#

if(POPPLER_QT4_FOUND)
    include_directories(${POPPLER_QT4_INCLUDE_DIR})

    kde4_add_plugin(kmetadata_popplerextractor popplerextractor.cpp)

    target_link_libraries(kmetadata_popplerextractor
        kmetadataextractor
        ${KDE4_KIO_LIBS}
        ${POPPLER_QT4_LIBRARIES}
    )

    install(
    FILES kmetadata_popplerextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kmetadata_popplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(POPPLER_QT4_FOUND)

if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    kde4_add_plugin( kmetadata_taglibextractor taglibextractor.cpp )

    target_link_libraries( kmetadata_taglibextractor
        kmetadataextractor
        ${KDE4_KIO_LIBS}
        ${TAGLIB_LIBRARIES}
    )

    install(
    FILES kmetadata_taglibextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kmetadata_taglibextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(TAGLIB_FOUND)

if(EXIV2_FOUND)
    include_directories( ${EXIV2_INCLUDE_DIR} )

    set_source_files_properties( exiv2extractor.cpp PROPERTIES
        COMPILE_FLAGS "${KDE4_ENABLE_EXCEPTIONS}"
        )

    kde4_add_plugin(kmetadata_exiv2extractor exiv2extractor.cpp)

    target_link_libraries(kmetadata_exiv2extractor
        kmetadataextractor
        ${KDE4_KIO_LIBS}
        ${EXIV2_LIBRARIES}
    )

    install(
    FILES kmetadata_exiv2extractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kmetadata_exiv2extractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(EXIV2_FOUND)

if(FFMPEG_FOUND)
    include_directories( ${FFMPEG_INCLUDE_DIRS} )

    kde4_add_plugin(kmetadata_ffmpegextractor ffmpegextractor.cpp)

    target_link_libraries(kmetadata_ffmpegextractor
        kmetadataextractor
        ${KDE4_KIO_LIBS}
        ${FFMPEG_LIBRARIES}
    )

    install(
    FILES kmetadata_ffmpegextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kmetadata_ffmpegextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(FFMPEG_FOUND)


if(EPUB_FOUND)
    include_directories( ${EPUB_INCLUDE_DIR} )

    kde4_add_plugin(kmetadata_epubextractor epubextractor.cpp)

    target_link_libraries(kmetadata_epubextractor
        kmetadataextractor
        ${KDE4_KIO_LIBS}
        ${EPUB_LIBRARIES}
    )

    install(
    FILES kmetadata_epubextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kmetadata_epubextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(EPUB_FOUND)

#
# Plain Text
#
kde4_add_plugin( kmetadata_plaintextextractor plaintextextractor.cpp )

target_link_libraries( kmetadata_plaintextextractor
    kmetadataextractor
    ${KDE4_KIO_LIBS}
    ${FFMPEG_LIBRARIES}
)

install(
FILES kmetadata_plaintextextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kmetadata_plaintextextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# ODF
#

kde4_add_plugin(kmetadata_odfextractor odfextractor.cpp)

target_link_libraries(kmetadata_odfextractor
    kmetadataextractor
    ${KDE4_KIO_LIBS}
)

install(
FILES kmetadata_odfextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kmetadata_odfextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Office 2007
#

kde4_add_plugin(kmetadata_office2007extractor office2007extractor.cpp)

target_link_libraries(kmetadata_office2007extractor
    kmetadataextractor
    ${KDE4_KIO_LIBS}
)

install(
FILES kmetadata_office2007extractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kmetadata_office2007extractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Office (binary formats)
#

kde4_add_plugin(kmetadata_officeextractor officeextractor.cpp)

target_link_libraries(kmetadata_officeextractor
    kmetadataextractor
    ${KDE4_KIO_LIBS}
)

install(
FILES kmetadata_officeextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS kmetadata_officeextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Mobipocket
#
if (QMOBIPOCKET_FOUND)
    add_subdirectory(mobipocket)
endif(QMOBIPOCKET_FOUND)

#add_subdirectory(test)
