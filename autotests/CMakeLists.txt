find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

find_package(PythonInterp)
set_package_properties(PythonInterp PROPERTIES DESCRIPTION "Python Interpreter"
                       URL "https://www.python.org" TYPE OPTIONAL
                       PURPOSE "Python interpreter is needed to execute test for external extractors or writers")

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

configure_file(indexerextractortestsconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/indexerextractortestsconfig.h @ONLY)


set(KfileMetaDataAutotest_SRCS)
ecm_qt_declare_logging_category(KfileMetaDataAutotest_SRCS
    HEADER kfilemetadata_debug.h
    IDENTIFIER KFILEMETADATA_LOG
    CATEGORY_NAME kf.filemetadata
)


#
# Test case coverage
#
set(extractorcoverage_SRCS
  extractorcoveragetest.cpp
)

ecm_add_test(${extractorcoverage_SRCS}
    TEST_NAME "extractorcoveragetest"
    LINK_LIBRARIES Qt5::Test KF5::FileMetaData
)

#
# Full text extraction test
#
set(indexerextractor_SRCS
  indexerextractortests.cpp
  ../src/extractors/plaintextextractor.cpp
)

ecm_add_test(${indexerextractor_SRCS}
    TEST_NAME "indexextractortest"
    LINK_LIBRARIES Qt5::Test KF5::FileMetaData
)

#
# Office
#
if(KF5Archive_FOUND)
    ecm_add_test(odfextractortest.cpp ../src/extractors/odfextractor.cpp
        TEST_NAME "odfextractortest"
        LINK_LIBRARIES Qt5::Test Qt5::Xml KF5::FileMetaData KF5::Archive
    )
endif()

if(KF5Archive_FOUND)
    ecm_add_test(office2007extractortest.cpp ../src/extractors/office2007extractor.cpp
        TEST_NAME "officeextractortest"
        LINK_LIBRARIES Qt5::Test Qt5::Xml KF5::FileMetaData KF5::Archive
    )
endif()

#
# Poppler
#
if(Poppler_Qt5_FOUND)
    ecm_add_test(popplerextractortest.cpp ../src/extractors/popplerextractor.cpp
        TEST_NAME "popplerextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData Poppler::Qt5
    )
endif()

#
# EPub
#
if(EPUB_FOUND)
    include_directories(${EPUB_INCLUDE_DIR})

    ecm_add_test(
        epubextractortest.cpp
	../src/extractors/epubextractor.cpp
	../src/kfilemetadata_debug.cpp
        TEST_NAME "epubextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${EPUB_LIBRARIES}
    )
endif()

#
# Mobi
#
if (QMobipocket_FOUND)
    ecm_add_test(mobiextractortest.cpp ../src/extractors/mobiextractor.cpp
        TEST_NAME "mobiextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData qmobipocket
    )
endif()

#
# Property Info
#
add_executable(propertyinfotest_bin propertyinfotest.cpp)
target_link_libraries(propertyinfotest_bin
    Qt5::Test
    KF5::FileMetaData
)
ecm_mark_as_test(propertyinfotest_bin)
ecm_mark_nongui_executable(propertyinfotest_bin)
add_test(NAME propertyinfotest_en COMMAND propertyinfotest_bin)
add_test(NAME propertyinfotest_localized COMMAND propertyinfotest_bin "--localized")

#
# Exiv2
#
if(LibExiv2_FOUND)
    kde_enable_exceptions()
    ecm_add_test(exiv2extractortest.cpp ../src/extractors/exiv2extractor.cpp
        TEST_NAME "exiv2extractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData LibExiv2::LibExiv2
    )
    if (${LibExiv2_VERSION} VERSION_GREATER 0.27.4)
        # exiv2 doesn't build [yet] with C++17 so force C++14
        message(AUTHOR_WARNING "hello developer of the future, please check if with > 0.27.4 we can stop forcing CXX_STANDARD 14")
    endif()
    set_target_properties(exiv2extractortest PROPERTIES CXX_STANDARD 14)
    target_include_directories(exiv2extractortest PRIVATE ${CMAKE_BINARY_DIR}/src/extractors)
endif()

#
# FFMPEG
#
if(FFMPEG_FOUND)
    kde_enable_exceptions()
    ecm_add_test(
        ffmpegextractortest.cpp
        ../src/extractors/ffmpegextractor.cpp
        ../src/kfilemetadata_debug.cpp
        TEST_NAME "ffmpegextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${AVCODEC_LIBRARIES} ${AVFORMAT_LIBRARIES} ${AVUTIL_LIBRARIES}
    )
    target_include_directories(ffmpegextractortest SYSTEM PRIVATE ${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS})
endif()

#
# TagLib
#
if(Taglib_FOUND)
    kde_enable_exceptions()
    ecm_add_test(taglibextractortest.cpp ../src/extractors/taglibextractor.cpp ${KfileMetaDataAutotest_SRCS}
        TEST_NAME "taglibextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData Taglib::Taglib
    )
    ecm_add_test(embeddedimagedatatest.cpp
        TEST_NAME "embeddedimagedatatest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData Taglib::Taglib
    )
endif()


if(PYTHONINTERP_FOUND)
    configure_file(samplefiles/testexternalextractor/main.py samplefiles/testexternalextractor/main.py)
    configure_file(samplefiles/testexternalextractor/manifest.json samplefiles/testexternalextractor/manifest.json COPYONLY)

    ecm_add_test(externalextractortest.cpp ../src/externalextractor.cpp ${KfileMetaDataAutotest_SRCS}
            TEST_NAME "externalextractortest"
            LINK_LIBRARIES Qt5::Test KF5::FileMetaData KF5::I18n
    )
endif()

#
# Collection
#
set(extractorcollection_SRCS
    extractorcollectiontest.cpp
)

ecm_add_test(${extractorcollection_SRCS}
        TEST_NAME "extractorcollectiontest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData
)

if(Taglib_FOUND)
    set(writercollection_SRCS
        writercollectiontest.cpp
    )

    ecm_add_test(${writercollection_SRCS}
            TEST_NAME "writercollectiontest"
            LINK_LIBRARIES Qt5::Test KF5::FileMetaData
    )
endif()

#
# XML
#
set(xmlextractor_SRCS
    xmlextractortest.cpp
    ../src/extractors/xmlextractor.cpp
    ../src/extractors/dublincoreextractor.cpp
    ../src/kfilemetadata_debug.cpp
)

ecm_add_test(${xmlextractor_SRCS}
        TEST_NAME "xmlextractortest"
        LINK_LIBRARIES Qt5::Test Qt5::Xml KF5::FileMetaData
)

#
# Postscript DSC
#
set(postscriptdscextractor_SRCS
    postscriptdscextractortest.cpp
    ../src/extractors/postscriptdscextractor.cpp
    ../src/kfilemetadata_debug.cpp
)

ecm_add_test(${postscriptdscextractor_SRCS}
        TEST_NAME "postscriptdscextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData
)

#
# AppImage
#
if(libappimage_FOUND AND KF5Config_FOUND AND Qt5Gui_FOUND)
    ecm_add_test(appimageextractortest.cpp ../src/extractors/appimageextractor.cpp
        TEST_NAME "appimageextractortest"
        LINK_LIBRARIES
            Qt5::Test
            KF5::FileMetaData
            KF5::ConfigCore
            Qt5::Xml
            Qt5::Gui
            libappimage
    )
endif()

################
# Writer tests #
################

#
# UserMetaData
#
if(UNIX)
    kde_enable_exceptions()
    ecm_add_test(usermetadatawritertest.cpp ../src/usermetadata.cpp
        TEST_NAME "usermetadatawritertest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData
    )
endif()

#
# TagLib
#
if(Taglib_FOUND)
    kde_enable_exceptions()
    ecm_add_test(taglibwritertest.cpp ../src/writers/taglibwriter.cpp ${KfileMetaDataAutotest_SRCS}
        TEST_NAME "taglibwritertest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData Taglib::Taglib
    )
endif()

if(PYTHONINTERP_FOUND)
    configure_file(samplefiles/testexternalwriter/main.py samplefiles/testexternalwriter/main.py)
    configure_file(samplefiles/testexternalwriter/manifest.json samplefiles/testexternalwriter/manifest.json COPYONLY)

    ecm_add_test(externalwritertest.cpp ../src/externalwriter.cpp ${KfileMetaDataAutotest_SRCS}
            TEST_NAME "externalwritertest"
            LINK_LIBRARIES Qt5::Test KF5::FileMetaData KF5::I18n
    )
endif()
