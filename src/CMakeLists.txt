ecm_qt_declare_logging_category(debug_SRCS
    HEADER kfilemetadata_debug.h
    IDENTIFIER KFILEMETADATA_LOG
    CATEGORY_NAME kf.filemetadata
    OLD_CATEGORY_NAMES kf5.kfilemetadata
    DESCRIPTION "KFileMetaData"
    EXPORT KFILEMETADATA
)

add_library(KF6FileMetaData)
add_library(KF6::FileMetaData ALIAS KF6FileMetaData)

set_target_properties(KF6FileMetaData PROPERTIES
    VERSION     ${KFILEMETADATA_VERSION}
    SOVERSION   ${KFILEMETADATA_SOVERSION}
    EXPORT_NAME FileMetaData
)

target_sources(KF6FileMetaData PRIVATE
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

target_link_libraries(KF6FileMetaData
  PUBLIC
    Qt6::Core
  PRIVATE
    KF6::I18n
    KF6::CoreAddons
)

ecm_generate_export_header(KF6FileMetaData
    EXPORT_FILE_NAME kfilemetadata_export.h
    BASE_NAME KFileMetaData
    GROUP_BASE_NAME KF
    VERSION ${KF_VERSION}
    USE_VERSION_HEADER
    DEPRECATED_BASE_VERSION 0
    DEPRECATION_VERSIONS 6.2 6.12
    EXCLUDE_DEPRECATED_BEFORE_AND_AT ${EXCLUDE_DEPRECATED_BEFORE_AND_AT}
)

target_include_directories(KF6FileMetaData INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF}/KFileMetaData>")

ecm_generate_headers(KF6FileMetaData_CamelCase_HEADERS
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

    PREFIX KFileMetaData
    REQUIRED_HEADERS KF6FileMetaData_HEADERS
)

ecm_generate_qdoc(KF6FileMetaData kfilemetadata.qdocconf)

install(TARGETS KF6FileMetaData EXPORT KF6FileMetaDataTargets ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

install(EXPORT KF6FileMetaDataTargets
        NAMESPACE KF6::
        DESTINATION ${KDE_INSTALL_LIBDIR}/cmake/KF6FileMetaData
        FILE KF6FileMetaDataTargets.cmake)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_export.h
    ${KF6FileMetaData_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KFileMetaData/kfilemetadata COMPONENT Devel
)

install(FILES
    ${KF6FileMetaData_CamelCase_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KFileMetaData/KFileMetaData COMPONENT Devel
)

configure_file(config-kfilemetadata.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-kfilemetadata.h)

add_subdirectory(extractors)

add_subdirectory(writers)

ecm_qt_install_logging_categories(
    EXPORT KFILEMETADATA
    FILE kfilemetadata.categories
    DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR}
)
