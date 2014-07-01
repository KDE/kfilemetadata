find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Test)

configure_file(indexerextractortestsconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/indexerextractortestsconfig.h @ONLY)

set(indexerextractor_SRCS
  indexerextractortests.cpp
  simpleresult.cpp
  ../src/extractors/plaintextextractor.cpp
)
ecm_add_test(${indexerextractor_SRCS}
    TEST_NAME "indexextractortest"
    LINK_LIBRARIES Qt5::Test KF5::FileMetaData KF5::Service
)

# Poppler
if(POPPLER_QT5_FOUND)
    include_directories(${POPPLER_QT5_INCLUDE_DIR})

    ecm_add_test(popplerextractortest.cpp simpleresult.cpp ../src/extractors/popplerextractor.cpp
        TEST_NAME "popplerextractortest"
        LINK_LIBRARIES Qt5::Test KF5::FileMetaData KF5::Service ${POPPLER_QT5_LIBRARIES}
    )
endif()

#
# Property Info
#
ecm_add_test(propertyinfotest.cpp
    TEST_NAME "propertyinfotest"
    LINK_LIBRARIES Qt5::Test KF5::FileMetaData
)
