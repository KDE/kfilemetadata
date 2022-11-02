

if(TARGET Poppler::Qt${QT_MAJOR_VERSION})
    add_library(kfilemetadata_popplerextractor MODULE popplerextractor.cpp)

    target_link_libraries(kfilemetadata_popplerextractor
        KF5::FileMetaData
        Poppler::Qt${QT_MAJOR_VERSION}
    )

    set_target_properties(kfilemetadata_popplerextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_popplerextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

endif()

if(Taglib_FOUND)
    add_library(kfilemetadata_taglibextractor MODULE taglibextractor.cpp ${debug_SRCS})
    target_link_libraries( kfilemetadata_taglibextractor
        KF5::FileMetaData
        Taglib::Taglib
    )

    set_target_properties(kfilemetadata_taglibextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_taglibextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)
endif()

if(LibExiv2_FOUND)
    add_library(kfilemetadata_exiv2extractor MODULE exiv2extractor.cpp)
    kde_target_enable_exceptions(kfilemetadata_exiv2extractor PRIVATE)
    target_link_libraries(kfilemetadata_exiv2extractor
        KF5::FileMetaData
        LibExiv2::LibExiv2
    )
    if(NOT ${LibExiv2_VERSION} VERSION_LESS 0.26)
        target_compile_definitions(kfilemetadata_exiv2extractor PRIVATE "-DHAVE_WEBP_SUPPORT")
        set(EXIV2_HAVE_WEBP_SUPPORT "1")
    endif()
    if(${LibExiv2_VERSION} VERSION_GREATER_EQUAL 0.27.4)
        set(EXIV2_HAVE_BMFF_SUPPORT "1")
    endif()
    configure_file(exiv2extractor.json.in exiv2extractor.json)

    if (${LibExiv2_VERSION} VERSION_GREATER 0.27.4)
        # exiv2 doesn't build [yet] with C++17 so force C++14
        message(AUTHOR_WARNING "hello developer of the future, please check if with > 0.27.4 we can stop forcing CXX_STANDARD 14")
    endif()
    set_target_properties(kfilemetadata_exiv2extractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata"
                                                                  CXX_STANDARD 14)
    install(
    TARGETS kfilemetadata_exiv2extractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

endif()

if(AVCODEC_FOUND AND AVFORMAT_FOUND AND AVUTIL_FOUND)
    add_library(kfilemetadata_ffmpegextractor MODULE
        ffmpegextractor.cpp
        ../kfilemetadata_debug.cpp
    )
    target_include_directories(kfilemetadata_ffmpegextractor SYSTEM PRIVATE ${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS})
    target_link_libraries(kfilemetadata_ffmpegextractor
        KF5::FileMetaData
	${AVCODEC_LIBRARIES}
	${AVFORMAT_LIBRARIES}
	${AVUTIL_LIBRARIES}
    )

    set_target_properties(kfilemetadata_ffmpegextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_ffmpegextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

endif()


if(EPUB_FOUND)
    add_library(kfilemetadata_epubextractor MODULE
        epubextractor.cpp
        ../kfilemetadata_debug.cpp
    )
    target_include_directories(kfilemetadata_epubextractor SYSTEM PRIVATE ${EPUB_INCLUDE_DIR})
    target_link_libraries(kfilemetadata_epubextractor
        KF5::FileMetaData
        ${EPUB_LIBRARIES}
    )

    set_target_properties(kfilemetadata_epubextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_epubextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

endif()

#
# Plain Text
#
add_library(kfilemetadata_plaintextextractor MODULE plaintextextractor.cpp)

target_link_libraries( kfilemetadata_plaintextextractor
    KF5::FileMetaData
)
if (TARGET Qt6::Core5Compat)
    target_link_libraries(kfilemetadata_plaintextextractor Qt6::Core5Compat) # QTextCodec
endif()

set_target_properties(kfilemetadata_plaintextextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
TARGETS kfilemetadata_plaintextextractor
DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

#
# PO
#
add_library(kfilemetadata_poextractor MODULE poextractor.cpp)
target_link_libraries( kfilemetadata_poextractor
    KF5::FileMetaData
)

set_target_properties(kfilemetadata_poextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
TARGETS kfilemetadata_poextractor
DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

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
    Qt${QT_MAJOR_VERSION}::Core
    Qt${QT_MAJOR_VERSION}::Xml
)

set_target_properties(kfilemetadata_xmlextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
    TARGETS kfilemetadata_xmlextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata
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
    Qt${QT_MAJOR_VERSION}::Core
)

set_target_properties(kfilemetadata_postscriptdscextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
    TARGETS kfilemetadata_postscriptdscextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata
)

#
# ODF
#

if(KF5Archive_FOUND)
    add_library(kfilemetadata_odfextractor MODULE odfextractor.cpp)

    target_link_libraries(kfilemetadata_odfextractor
        KF5::FileMetaData
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Xml
        KF5::Archive
    )

    set_target_properties(kfilemetadata_odfextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_odfextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)
endif()

#
# Office 2007
#

if(KF5Archive_FOUND)
    add_library(kfilemetadata_office2007extractor MODULE office2007extractor.cpp)

    target_link_libraries(kfilemetadata_office2007extractor
        KF5::FileMetaData
        Qt${QT_MAJOR_VERSION}::Core
        Qt${QT_MAJOR_VERSION}::Xml
        KF5::Archive
    )

    set_target_properties(kfilemetadata_office2007extractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_office2007extractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)
endif()

#
# Office (binary formats)
#

add_library(kfilemetadata_officeextractor MODULE)
target_sources(kfilemetadata_officeextractor
    PRIVATE
        officeextractor.cpp
        ${debug_SRCS}
)

target_link_libraries(kfilemetadata_officeextractor
    KF5::FileMetaData
)

set_target_properties(kfilemetadata_officeextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
TARGETS kfilemetadata_officeextractor
DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

#
# Mobipocket
#
if (QMobipocket_FOUND)
    add_library(kfilemetadata_mobiextractor MODULE mobiextractor.cpp)
    target_link_libraries(kfilemetadata_mobiextractor
        KF5::FileMetaData
        qmobipocket
    )

    set_target_properties(kfilemetadata_mobiextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_mobiextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

endif()

if(libappimage_FOUND AND KF5Config_FOUND AND Qt${QT_MAJOR_VERSION}Gui_FOUND)
    add_library(kfilemetadata_appimageextractor MODULE appimageextractor.cpp )
    target_link_libraries( kfilemetadata_appimageextractor
        KF5::FileMetaData
        KF5::ConfigCore
        Qt${QT_MAJOR_VERSION}::Xml
        Qt${QT_MAJOR_VERSION}::Gui
        libappimage
    )

    set_target_properties(kfilemetadata_appimageextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
    install(
    TARGETS kfilemetadata_appimageextractor
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)
endif()

add_library(kfilemetadata_pngextractor MODULE pngextractor.cpp )
target_link_libraries(kfilemetadata_pngextractor
    KF5::FileMetaData
    Qt${QT_MAJOR_VERSION}::Gui
)

set_target_properties(kfilemetadata_pngextractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
TARGETS kfilemetadata_pngextractor
DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)

#
# FictionBook2
#
add_library(kfilemetadata_fb2extractor MODULE fb2extractor.cpp )
target_link_libraries(kfilemetadata_fb2extractor
    KF5::Archive
    KF5::FileMetaData
    Qt${QT_MAJOR_VERSION}::Core
)

set_target_properties(kfilemetadata_fb2extractor PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata")
install(
TARGETS kfilemetadata_fb2extractor
DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata)
