add_library(KF5FileMetaData
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
)

add_library(KF5::FileMetaData ALIAS KF5FileMetaData)

target_link_libraries(KF5FileMetaData
  PUBLIC
    Qt5::Core
  PRIVATE
    KF5::I18n
)

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

add_subdirectory(extractors)

add_subdirectory(writers)

