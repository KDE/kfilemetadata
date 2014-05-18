add_library(KF5FileMetaData SHARED
    extractionresult.cpp
    extractorplugin.cpp
    extractorpluginmanager.cpp
    propertyinfo.cpp
    typeinfo.cpp
)

add_library(KF5::FileMetaData ALIAS KF5FileMetaData)

target_link_libraries(KF5FileMetaData
  PUBLIC
    Qt5::Core
  PRIVATE
    KF5::Service
    KF5::I18n
)

generate_export_header(KF5FileMetaData BASE_NAME KFileMetaData EXPORT_FILE_NAME kfilemetadata_export.h)
set_target_properties(KF5FileMetaData PROPERTIES
    VERSION ${KFILEMETADATA_VERSION_STRING}
    SOVERSION ${KFILEMETADATA_SOVERSION}
    EXPORT_NAME FileMetaData
)
target_include_directories(KF5FileMetaData INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/KFileMetaData>")

ecm_generate_headers(KF5FileMetaData_HEADERS
    HEADER_NAMES
    ExtractionResult
    ExtractorPlugin
    ExtractorPluginManager
    Properties
    PropertyInfo
    Types
    TypeInfo

    REQUIRED_HEADERS KF5FileMetaData_HEADERS
)

install(TARGETS KF5FileMetaData ${INSTALL_TARGETS_DEFAULT_ARGS})

install(TARGETS KF5FileMetaData EXPORT KFileMetaDataTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kfilemetadataextractor.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install(EXPORT KFileMetaDataTargets
        NAMESPACE KF5::
        DESTINATION ${LIB_INSTALL_DIR}/cmake/KF5FileMetaData
        FILE KF5FileMetaDataTargetsWithPrefix.cmake)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_export.h
    ${KF5FileMetaData_HEADERS}
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/KFileMetaData COMPONENT Devel
)

add_subdirectory(extractors)
