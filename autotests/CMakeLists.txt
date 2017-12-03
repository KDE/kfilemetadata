find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

find_package(PythonInterp)
set_package_properties(PythonInterp PROPERTIES DESCRIPTION "Python Interpreter"
                       URL "https://www.python.org" TYPE OPTIONAL
                       PURPOSE "Python interpreter is needed to execute test for external extractors or writers")

remove_definitions(-DQT_NO_CAST_FROM_ASCII)

configure_file(indexerextractortestsconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/indexerextractortestsconfig.h @ONLY)


set(KfileMetaDataAutotest_SRCS)
ecm_qt_declare_logging_category(KfileMetaDataAutotest_SRCS HEADER kfilemetadata_debug.h IDENTIFIER KFILEMETADATA_LOG CATEGORY_NAME kf5.kfilemetadata)


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

    ecm_add_test(epubextractortest.cpp ../src/extractors/epubextractor.cpp
        TEST_NAME "epubextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${EPUB_LIBRARIES}
    )
endif()

#
# Mobi
#
if (QMOBIPOCKET_FOUND)
    include_directories(${QMOBIPOCKET_INCLUDE_DIR})

    ecm_add_test(mobiextractortest.cpp ../src/extractors/mobiextractor.cpp
        TEST_NAME "mobiextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${QMOBIPOCKET_LIBRARIES}
    )
endif()

#
# Property Info
#
ecm_add_test(propertyinfotest.cpp
    TEST_NAME "propertyinfotest"
    LINK_LIBRARIES Qt5::Test KF5::FileMetaData
)

#
# Exiv2
#
if(EXIV2_FOUND)
    include_directories(${EXIV2_INCLUDE_DIR})

    kde_enable_exceptions()
    ecm_add_test(exiv2extractortest.cpp ../src/extractors/exiv2extractor.cpp
        TEST_NAME "exiv2extractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${EXIV2_LIBRARIES}
    )
endif()

#
# TagLib
#
if(TAGLIB_FOUND)
    include_directories(${TAGLIB_INCLUDES})

    kde_enable_exceptions()
    ecm_add_test(taglibextractortest.cpp ../src/extractors/taglibextractor.cpp
        TEST_NAME "taglibextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${TAGLIB_LIBRARIES}
    )
endif()

#
# QtMultimedia
#
if(NOT TAGLIB_FOUND AND Qt5Multimedia_FOUND)
    kde_enable_exceptions()
    ecm_add_test(qtmultimediaextractortest.cpp ../src/extractors/qtmultimediaextractor.cpp
        TEST_NAME "qtmultimediaextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData Qt5::Multimedia
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

################
# Writer tests #
################

#
# UserMetaData
#
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    kde_enable_exceptions()
    ecm_add_test(usermetadatawritertest.cpp ../src/usermetadata.cpp
        TEST_NAME "usermetadatawritertest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData
    )
endif()

#
# TagLib
#
if(TAGLIB_FOUND)
    include_directories(${TAGLIB_INCLUDES})

    kde_enable_exceptions()
    ecm_add_test(taglibwritertest.cpp ../src/writers/taglibwriter.cpp
        TEST_NAME "taglibwritertest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${TAGLIB_LIBRARIES}
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
