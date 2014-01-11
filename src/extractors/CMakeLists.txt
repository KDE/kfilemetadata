if(POPPLER_QT5_FOUND)
    include_directories(${POPPLER_QT5_INCLUDE_DIR})

    add_library(kfilemetadata_popplerextractor MODULE popplerextractor.cpp)

    target_link_libraries(kfilemetadata_popplerextractor
        kfilemetadata
        KF5::KIOWidgets
        KF5::Service
        KF5::KDE4Support
        ${POPPLER_QT5_LIBRARIES}
    )

    install(
    FILES kfilemetadata_popplerextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_popplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif(POPPLER_QT5_FOUND)

if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    add_library(kfilemetadata_taglibextractor MODULE taglibextractor.cpp )

    target_link_libraries( kfilemetadata_taglibextractor
        kfilemetadata
        KF5::KIOWidgets
        KF5::Service
        KF5::KDE4Support
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

    add_library(kfilemetadata_exiv2extractor MODULE exiv2extractor.cpp)

    target_link_libraries(kfilemetadata_exiv2extractor
        kfilemetadata
        KF5::KIOWidgets
        KF5::Service
        KF5::KDE4Support
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

    add_library(kfilemetadata_ffmpegextractor MODULE ffmpegextractor.cpp)

    target_link_libraries(kfilemetadata_ffmpegextractor
        kfilemetadata
        KF5::KIOWidgets
        KF5::Service
        KF5::KDE4Support
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

    add_library(kfilemetadata_epubextractor MODULE epubextractor.cpp)

    target_link_libraries(kfilemetadata_epubextractor
        kfilemetadata
        KF5::KIOWidgets
        KF5::Service
        KF5::KDE4Support
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
add_library(kfilemetadata_plaintextextractor MODULE plaintextextractor.cpp )

target_link_libraries( kfilemetadata_plaintextextractor
    kfilemetadata
    KF5::KIOWidgets
    KF5::Service
    KF5::KDE4Support
    ${FFMPEG_LIBRARIES}
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

add_library(kfilemetadata_odfextractor MODULE odfextractor.cpp)

target_link_libraries(kfilemetadata_odfextractor
    kfilemetadata
    Qt5::Core
    Qt5::Xml
    KF5::KIOWidgets
    KF5::Service
    KF5::KDE4Support
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

add_library(kfilemetadata_office2007extractor MODULE office2007extractor.cpp)

target_link_libraries(kfilemetadata_office2007extractor
    kfilemetadata
    Qt5::Core
    Qt5::Xml
    KF5::KIOWidgets
    KF5::Service
    KF5::KDE4Support
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

add_library(kfilemetadata_officeextractor MODULE officeextractor.cpp)

target_link_libraries(kfilemetadata_officeextractor
    kfilemetadata
    KF5::KIOWidgets
    KF5::Service
    KF5::KDE4Support
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
    add_library(kfilemetadata_mobiextractor MODULE mobiextractor.cpp)

    include_directories(${QMOBIPOCKET_INCLUDE_DIR})
    target_link_libraries(kfilemetadata_mobiextractor
        kfilemetadata
        KF5::KIOWidgets
        KF5::Service
        KF5::KDE4Support
        ${QMOBIPOCKET_LIBRARIES}
    )

    install(
    FILES kfilemetadata_mobiextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_mobiextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})
endif(QMOBIPOCKET_FOUND)
