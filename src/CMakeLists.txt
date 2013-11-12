project(nepomukindexer)

include_directories(
  ${QT_INCLUDES}
  ${KDE4_INCLUDES}
  ${SOPRANO_INCLUDE_DIR}
  )

kde4_add_library(nepomukextractor SHARED extractorplugin.cpp)

target_link_libraries(nepomukextractor
  ${QT_QTCORE_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  nepomukcore
)

install(TARGETS nepomukextractor EXPORT NepomukCoreLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

# -----------------------------
# Extractors
#

if(POPPLER_QT4_FOUND)
    include_directories( ${POPPLER_QT4_INCLUDE_DIR} )

    kde4_add_plugin( nepomukpopplerextractor popplerextractor.cpp )

    target_link_libraries( nepomukpopplerextractor
        nepomukcore
        nepomukextractor
        ${KDE4_KIO_LIBS}
        ${POPPLER_QT4_LIBRARIES}
    )

    install(
    FILES nepomukpopplerextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS nepomukpopplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(POPPLER_QT4_FOUND)

if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    kde4_add_plugin( nepomuktaglibextractor taglibextractor.cpp )

    target_link_libraries( nepomuktaglibextractor
        nepomukcore
        nepomukextractor
        ${KDE4_KIO_LIBS}
        ${TAGLIB_LIBRARIES}
    )

    install(
    FILES nepomuktaglibextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS nepomuktaglibextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(TAGLIB_FOUND)

if(EXIV2_FOUND)
    include_directories( ${EXIV2_INCLUDE_DIR} )

    set_source_files_properties( exiv2extractor.cpp PROPERTIES
        COMPILE_FLAGS "${KDE4_ENABLE_EXCEPTIONS}"
        )

    kde4_add_plugin( nepomukexiv2extractor exiv2extractor.cpp )

    target_link_libraries( nepomukexiv2extractor
        nepomukcore
        nepomukextractor
        ${KDE4_KIO_LIBS}
        ${EXIV2_LIBRARIES}
    )

    install(
    FILES nepomukexiv2extractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS nepomukexiv2extractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(EXIV2_FOUND)

if(FFMPEG_FOUND)
    include_directories( ${FFMPEG_INCLUDE_DIRS} )

    kde4_add_plugin( nepomukffmpegextractor ffmpegextractor.cpp )

    target_link_libraries( nepomukffmpegextractor
        nepomukcore
        nepomukextractor
        ${KDE4_KIO_LIBS}
        ${FFMPEG_LIBRARIES}
    )

    install(
    FILES nepomukffmpegextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS nepomukffmpegextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(FFMPEG_FOUND)


if(EPUB_FOUND)
    include_directories( ${EPUB_INCLUDE_DIR} )

    kde4_add_plugin( nepomukepubextractor epubextractor.cpp )

    target_link_libraries( nepomukepubextractor
        nepomukcore
        nepomukextractor
        ${KDE4_KIO_LIBS}
        ${EPUB_LIBRARIES}
    )

    install(
    FILES nepomukepubextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS nepomukepubextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(EPUB_FOUND)

#
# Plain Text
#
kde4_add_plugin( nepomukplaintextextractor plaintextextractor.cpp )

target_link_libraries( nepomukplaintextextractor
    nepomukcore
    nepomukextractor
    ${KDE4_KIO_LIBS}
    ${FFMPEG_LIBRARIES}
)

install(
FILES nepomukplaintextextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS nepomukplaintextextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# ODF
#

kde4_add_plugin( nepomukodfextractor odfextractor.cpp )

target_link_libraries( nepomukodfextractor
    nepomukcore
    nepomukextractor
    ${KDE4_KIO_LIBS}
)

install(
FILES nepomukodfextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS nepomukodfextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Office 2007
#

kde4_add_plugin( nepomukoffice2007extractor office2007extractor.cpp )

target_link_libraries( nepomukoffice2007extractor
    nepomukcore
    nepomukextractor
    ${KDE4_KIO_LIBS}
)

install(
FILES nepomukoffice2007extractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS nepomukoffice2007extractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Office (binary formats)
#

kde4_add_plugin( nepomukofficeextractor officeextractor.cpp )

target_link_libraries( nepomukofficeextractor
    nepomukcore
    nepomukextractor
    ${KDE4_KIO_LIBS}
)

install(
FILES nepomukofficeextractor.desktop
DESTINATION ${SERVICES_INSTALL_DIR})

install(
TARGETS nepomukofficeextractor
DESTINATION ${PLUGIN_INSTALL_DIR})

#
# Mobipocket
#
if (QMOBIPOCKET_FOUND)
    add_subdirectory(mobipocket)
endif(QMOBIPOCKET_FOUND)

#add_subdirectory(test)
