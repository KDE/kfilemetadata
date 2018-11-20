

if(Poppler_Qt5_FOUND)
    add_library(kfilemetadata_popplerextractor MODULE popplerextractor.cpp)

    target_link_libraries(kfilemetadata_popplerextractor
        KF5::FileMetaData
        Poppler::Qt5
    )

    set_target_properties(kfilemetadata_popplerextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
    install(
    TARGETS kfilemetadata_popplerextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

if(TAGLIB_FOUND)
    add_library(kfilemetadata_taglibextractor MODULE taglibextractor.cpp )
    target_include_directories(kfilemetadata_taglibextractor SYSTEM PRIVATE ${TAGLIB_INCLUDES})
    target_link_libraries( kfilemetadata_taglibextractor
        KF5::FileMetaData
        ${TAGLIB_LIBRARIES}
    )

    set_target_properties(kfilemetadata_taglibextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
    install(
    TARGETS kfilemetadata_taglibextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)
endif()

if(Exiv2_FOUND)
    add_library(kfilemetadata_exiv2extractor MODULE exiv2extractor.cpp)
    kde_target_enable_exceptions(kfilemetadata_exiv2extractor PRIVATE)
    target_link_libraries(kfilemetadata_exiv2extractor
        KF5::FileMetaData
        Exiv2::Exiv2
    )

    set_target_properties(kfilemetadata_exiv2extractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
    install(
    TARGETS kfilemetadata_exiv2extractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()

if(FFMPEG_FOUND)
    add_library(kfilemetadata_ffmpegextractor MODULE ffmpegextractor.cpp)
    target_include_directories(kfilemetadata_ffmpegextractor SYSTEM PRIVATE ${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS})
    target_link_libraries(kfilemetadata_ffmpegextractor
        KF5::FileMetaData
	${AVCODEC_LIBRARIES}
	${AVFORMAT_LIBRARIES}
	${AVUTIL_LIBRARIES}
    )

    set_target_properties(kfilemetadata_ffmpegextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
    install(
    TARGETS kfilemetadata_ffmpegextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()


if(EPUB_FOUND)
    add_library(kfilemetadata_epubextractor MODULE epubextractor.cpp)
    target_include_directories(kfilemetadata_epubextractor SYSTEM PRIVATE ${EPUB_INCLUDE_DIR})
    target_link_libraries(kfilemetadata_epubextractor
        KF5::FileMetaData
        ${EPUB_LIBRARIES}
    )

    set_target_properties(kfilemetadata_epubextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
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

set_target_properties(kfilemetadata_plaintextextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
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

set_target_properties(kfilemetadata_poextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
install(
TARGETS kfilemetadata_poextractor
DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

#
# XML
#
add_library(kfilemetadata_xmlextractor MODULE
   dublincoreextractor.cpp
   xmlextractor.cpp
   ../kfilemetadata_debug.cpp
)
target_link_libraries( kfilemetadata_xmlextractor
    KF5::FileMetaData
    Qt5::Core
    Qt5::Xml
)

set_target_properties(kfilemetadata_xmlextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
install(
    TARGETS kfilemetadata_xmlextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata
)

#
# Postscript DSC
#
add_library(kfilemetadata_postscriptdscextractor MODULE
   postscriptdscextractor.cpp
   ../kfilemetadata_debug.cpp
)
target_link_libraries( kfilemetadata_postscriptdscextractor
    KF5::FileMetaData
    Qt5::Core
)

set_target_properties(kfilemetadata_postscriptdscextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
install(
    TARGETS kfilemetadata_postscriptdscextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata
)

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

    set_target_properties(kfilemetadata_odfextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
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

    set_target_properties(kfilemetadata_office2007extractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
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

set_target_properties(kfilemetadata_officeextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
install(
TARGETS kfilemetadata_officeextractor
DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

#
# Mobipocket
#
if (QMOBIPOCKET_FOUND)
    add_library(kfilemetadata_mobiextractor MODULE mobiextractor.cpp)
    target_include_directories(kfilemetadata_mobiextractor SYSTEM PRIVATE ${QMOBIPOCKET_INCLUDE_DIR})
    target_link_libraries(kfilemetadata_mobiextractor
        KF5::FileMetaData
        ${QMOBIPOCKET_LIBRARIES}
    )

    set_target_properties(kfilemetadata_mobiextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata")
    install(
    TARGETS kfilemetadata_mobiextractor
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata)

endif()
