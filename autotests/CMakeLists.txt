find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

configure_file(indexerextractortestsconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/indexerextractortestsconfig.h @ONLY)

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
ecm_add_test(odfextractortest.cpp ../src/extractors/odfextractor.cpp
    TEST_NAME "odfextractortest"
    LINK_LIBRARIES Qt5::Test Qt5::Xml KF5::FileMetaData KF5::Archive
)

ecm_add_test(office2007extractortest.cpp ../src/extractors/office2007extractor.cpp
    TEST_NAME "officeextractortest"
    LINK_LIBRARIES Qt5::Test Qt5::Xml KF5::FileMetaData KF5::Archive
)

#
# Poppler
#
if(POPPLER_QT5_FOUND)
    include_directories(${POPPLER_QT5_INCLUDE_DIR})

    ecm_add_test(popplerextractortest.cpp ../src/extractors/popplerextractor.cpp
        TEST_NAME "popplerextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData ${POPPLER_QT5_LIBRARIES}
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
