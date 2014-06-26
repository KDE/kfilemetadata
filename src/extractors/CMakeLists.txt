if(POPPLER_QT5_FOUND)
    include_directories(${POPPLER_QT5_INCLUDE_DIR})

    add_library(kfilemetadata_popplerextractor MODULE popplerextractor.cpp)

    target_link_libraries(kfilemetadata_popplerextractor
        KF5::FileMetaData
        KF5::Service
        ${POPPLER_QT5_LIBRARIES}
    )

    install(
    FILES kfilemetadata_popplerextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_popplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif()

if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    add_library(kfilemetadata_taglibextractor MODULE taglibextractor.cpp )

    target_link_libraries( kfilemetadata_taglibextractor
        KF5::FileMetaData
        KF5::Service
        ${TAGLIB_LIBRARIES}
    )

    install(
    FILES kfilemetadata_taglibextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_taglibextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif()

if(EXIV2_FOUND)
    include_directories( ${EXIV2_INCLUDE_DIR} )

    kde_enable_exceptions()
    add_library(kfilemetadata_exiv2extractor MODULE exiv2extractor.cpp)

    target_link_libraries(kfilemetadata_exiv2extractor
        KF5::FileMetaData
        KF5::Service
        ${EXIV2_LIBRARIES}
    )

    install(
    FILES kfilemetadata_exiv2extractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_exiv2extractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif()

if(FFMPEG_FOUND)
    include_directories( ${FFMPEG_INCLUDE_DIRS} )

    add_library(kfilemetadata_ffmpegextractor MODULE ffmpegextractor.cpp)

    target_link_libraries(kfilemetadata_ffmpegextractor
        KF5::FileMetaData
        KF5::Service
        ${FFMPEG_LIBRARIES}
    )

    install(
    FILES kfilemetadata_ffmpegextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_ffmpegextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif()


if(EPUB_FOUND)
    include_directories( ${EPUB_INCLUDE_DIR} )

    add_library(kfilemetadata_epubextractor MODULE epubextractor.cpp)

    target_link_libraries(kfilemetadata_epubextractor
        KF5::FileMetaData
        KF5::Service
        ${EPUB_LIBRARIES}
    )

    install(
    FILES kfilemetadata_epubextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_epubextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif()

#
# Plain Text
#
add_library(kfilemetadata_plaintextextractor MODULE plaintextextractor.cpp)

target_link_libraries( kfilemetadata_plaintextextractor
    KF5::FileMetaData
    KF5::Service
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
    KF5::FileMetaData
    Qt5::Core
    Qt5::Xml
    KF5::Service
    KF5::Archive
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
    KF5::FileMetaData
    Qt5::Core
    Qt5::Xml
    KF5::Service
    KF5::Archive
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
    KF5::FileMetaData
    KF5::Service
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
        KF5::FileMetaData
        KF5::Service
        ${QMOBIPOCKET_LIBRARIES}
    )

    install(
    FILES kfilemetadata_mobiextractor.desktop
    DESTINATION ${SERVICES_INSTALL_DIR})

    install(
    TARGETS kfilemetadata_mobiextractor
    DESTINATION ${PLUGIN_INSTALL_DIR})

endif()
