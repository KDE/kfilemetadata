if(Poppler_Qt5_FOUND)
    add_library(kfilemetadata_popplerextractor MODULE popplerextractor.cpp)

    target_link_libraries(kfilemetadata_popplerextractor
        KF5::FileMetaData
        Poppler::Qt5
    )

    install(
    TARGETS kfilemetadata_popplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    add_library(kfilemetadata_taglibextractor MODULE taglibextractor.cpp )

    target_link_libraries( kfilemetadata_taglibextractor
        KF5::FileMetaData
        ${TAGLIB_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_taglibextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

if(NOT TAGLIB_FOUND AND Qt5Multimedia_FOUND)
    add_library(kfilemetadata_qtmultimediaextractor MODULE qtmultimediaextractor.cpp )

    target_link_libraries( kfilemetadata_qtmultimediaextractor
        KF5::FileMetaData
        Qt5::Multimedia
    )

    install(
    TARGETS kfilemetadata_qtmultimediaextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

if(EXIV2_FOUND)
    include_directories( ${EXIV2_INCLUDE_DIR} )

    kde_enable_exceptions()
    add_library(kfilemetadata_exiv2extractor MODULE exiv2extractor.cpp)

    target_link_libraries(kfilemetadata_exiv2extractor
        KF5::FileMetaData
        ${EXIV2_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_exiv2extractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

if(FFMPEG_FOUND)
    include_directories( ${FFMPEG_INCLUDE_DIRS} )

    add_library(kfilemetadata_ffmpegextractor MODULE ffmpegextractor.cpp)

    target_link_libraries(kfilemetadata_ffmpegextractor
        KF5::FileMetaData
        ${FFMPEG_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_ffmpegextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()


if(EPUB_FOUND)
    include_directories( ${EPUB_INCLUDE_DIR} )

    add_library(kfilemetadata_epubextractor MODULE epubextractor.cpp)

    target_link_libraries(kfilemetadata_epubextractor
        KF5::FileMetaData
        ${EPUB_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_epubextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

#
# Plain Text
#
add_library(kfilemetadata_plaintextextractor MODULE plaintextextractor.cpp)

target_link_libraries( kfilemetadata_plaintextextractor
    KF5::FileMetaData
)

install(
TARGETS kfilemetadata_plaintextextractor
DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

#
# PO
#
add_library(kfilemetadata_poextractor MODULE poextractor.cpp)
target_link_libraries( kfilemetadata_poextractor
    KF5::FileMetaData
)

install(
TARGETS kfilemetadata_poextractor
DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

#
# ODF
#

if(KF5Archive_FOUND)
    add_library(kfilemetadata_odfextractor MODULE odfextractor.cpp)

    target_link_libraries(kfilemetadata_odfextractor
        KF5::FileMetaData
        Qt5::Core
        Qt5::Xml
        KF5::Archive
    )

    install(
    TARGETS kfilemetadata_odfextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)
endif()

#
# Office 2007
#

if(KF5Archive_FOUND)
    add_library(kfilemetadata_office2007extractor MODULE office2007extractor.cpp)

    target_link_libraries(kfilemetadata_office2007extractor
        KF5::FileMetaData
        Qt5::Core
        Qt5::Xml
        KF5::Archive
    )

    install(
    TARGETS kfilemetadata_office2007extractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)
endif()

#
# Office (binary formats)
#

add_library(kfilemetadata_officeextractor MODULE officeextractor.cpp)

target_link_libraries(kfilemetadata_officeextractor
    KF5::FileMetaData
)

install(
TARGETS kfilemetadata_officeextractor
DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

#
# Mobipocket
#
if (QMOBIPOCKET_FOUND)
    add_library(kfilemetadata_mobiextractor MODULE mobiextractor.cpp)

    include_directories(${QMOBIPOCKET_INCLUDE_DIR})
    target_link_libraries(kfilemetadata_mobiextractor
        KF5::FileMetaData
        ${QMOBIPOCKET_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_mobiextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()
