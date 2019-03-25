set(KF5FileMetaData_SRCS
    extractionresult.cpp
    simpleextractionresult.cpp
    extractor.cpp
    extractorplugin.cpp
    extractorcollection.cpp
    externalextractor.cpp
    propertyinfo.cpp
    typeinfo.cpp
    usermetadata.cpp
    writedata.cpp
    writer.cpp
    writerplugin.cpp
    writercollection.cpp
    externalwriter.cpp
    formatstrings.cpp
    mimeutils.cpp
    )
ecm_qt_declare_logging_category(KF5FileMetaData_SRCS HEADER kfilemetadata_debug.h IDENTIFIER KFILEMETADATA_LOG CATEGORY_NAME kf5.kfilemetadata)

if(TAGLIB_FOUND)
    set(KF5FileMetaData_SRCS
        ${KF5FileMetaData_SRCS}
        embeddedimagedata.cpp
        )
endif()

add_library(KF5FileMetaData
    ${KF5FileMetaData_SRCS}
)

add_library(KF5::FileMetaData ALIAS KF5FileMetaData)

target_link_libraries(KF5FileMetaData
  PUBLIC
    Qt5::Core
  PRIVATE
    KF5::I18n
    KF5::CoreAddons
)

if(TAGLIB_FOUND)
    target_include_directories(KF5FileMetaData SYSTEM PRIVATE ${TAGLIB_INCLUDES})

    target_link_libraries(KF5FileMetaData
        PRIVATE
        ${TAGLIB_LIBRARIES}
    )
endif()

generate_export_header(KF5FileMetaData BASE_NAME KFileMetaData EXPORT_FILE_NAME kfilemetadata_export.h)

set_target_properties(KF5FileMetaData PROPERTIES
    VERSION ${KFILEMETADATA_VERSION_STRING}
    SOVERSION ${KFILEMETADATA_SOVERSION}
    EXPORT_NAME FileMetaData
)

target_include_directories(KF5FileMetaData INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KFileMetaData>")

ecm_generate_headers(KF5FileMetaData_CamelCase_HEADERS
    HEADER_NAMES
    ExtractionResult
    SimpleExtractionResult
    Extractor
    ExtractorPlugin
    ExtractorCollection
    Properties
    PropertyInfo
    Types
    TypeInfo
    UserMetaData
    WriteData
    Writer
    WriterPlugin
    WriterCollection
    EmbeddedImageData

    PREFIX kfilemetadata
    REQUIRED_HEADERS KF5FileMetaData_HEADERS
)

install(TARGETS KF5FileMetaData EXPORT KF5FileMetaDataTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(EXPORT KF5FileMetaDataTargets
        NAMESPACE KF5::
        DESTINATION ${LIB_INSTALL_DIR}/cmake/KF5FileMetaData
        FILE KF5FileMetaDataTargets.cmake)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_export.h
    ${KF5FileMetaData_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KFileMetaData/kfilemetadata COMPONENT Devel
)

install(FILES
    ${KF5FileMetaData_CamelCase_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KFileMetaData/KFileMetaData COMPONENT Devel
)

configure_file(config-kfilemetadata.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-kfilemetadata.h)

if(BUILD_QCH)
    ecm_add_qch(
        KF5FileMetaData_QCH
        NAME KFileMetaData
        BASE_NAME KF5FileMetaData
        VERSION ${KF5_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KF5FileMetaData_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
        BLANK_MACROS
            KFILEMETADATA_EXPORT
            KFILEMETADATA_DEPRECATED
            KFILEMETADATA_DEPRECATED_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(
    BASE_NAME KFileMetaData
    LIB_NAME KF5FileMetaData
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KF5_INCLUDE_INSTALL_DIR}/KFileMetaData
)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

add_subdirectory(extractors)

add_subdirectory(writers)

