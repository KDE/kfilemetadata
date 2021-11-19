ecm_qt_declare_logging_category(debug_SRCS
    HEADER kfilemetadata_debug.h
    IDENTIFIER KFILEMETADATA_LOG
    CATEGORY_NAME kf.filemetadata
    OLD_CATEGORY_NAMES kf5.kfilemetadata
    DESCRIPTION "KFileMetaData"
    EXPORT KFILEMETADATA
)

set(KF5FileMetaData_SRCS
    extractionresult.cpp
    simpleextractionresult.cpp
    embeddedimagedata.cpp
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
    ${debug_SRCS}
    )

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

ecm_generate_export_header(KF5FileMetaData
    EXPORT_FILE_NAME kfilemetadata_export.h
    BASE_NAME KFileMetaData
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 5.50 5.60 5.76 5.82 5.89
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

set_target_properties(KF5FileMetaData PROPERTIES
    VERSION ${KFILEMETADATA_VERSION}
    SOVERSION ${KFILEMETADATA_SOVERSION}
    EXPORT_NAME FileMetaData
)

target_include_directories(KF5FileMetaData INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KFileMetaData>")

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
    MimeUtils

    PREFIX kfilemetadata
    REQUIRED_HEADERS KF5FileMetaData_HEADERS
)

install(TARGETS KF5FileMetaData EXPORT KF5FileMetaDataTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

install(EXPORT KF5FileMetaDataTargets
        NAMESPACE KF5::
        DESTINATION ${KDE_INSTALL_LIBDIR}/cmake/KF5FileMetaData
        FILE KF5FileMetaDataTargets.cmake)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_export.h
    ${KF5FileMetaData_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KFileMetaData/kfilemetadata COMPONENT Devel
)

install(FILES
    ${KF5FileMetaData_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KFileMetaData/KFileMetaData COMPONENT Devel
)

configure_file(config-kfilemetadata.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-kfilemetadata.h)

if(BUILD_QCH)
    ecm_add_qch(
        KF5FileMetaData_QCH
        NAME KFileMetaData
        BASE_NAME KF5FileMetaData
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KF5FileMetaData_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KFILEMETADATA_EXPORT
            KFILEMETADATA_DEPRECATED
            KFILEMETADATA_DEPRECATED_EXPORT
            "KFILEMETADATA_DEPRECATED_VERSION(x, y, t)"
            "KFILEMETADATA_DEPRECATED_VERSION_BELATED(x, y, xt, yt, t)"
            "KFILEMETADATA_ENUMERATOR_DEPRECATED_VERSION(x, y, t)"
            "KFILEMETADATA_ENUMERATOR_DEPRECATED_VERSION_BELATED(x, y, xt, yt, t)"
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
    INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KFileMetaData
)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

add_subdirectory(extractors)

add_subdirectory(writers)

ecm_qt_install_logging_categories(
    EXPORT KFILEMETADATA
    FILE kfilemetadata.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)
