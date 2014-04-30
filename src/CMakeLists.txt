add_library(kfilemetadata SHARED
    extractionresult.cpp
    extractorplugin.cpp
    extractorpluginmanager.cpp
    propertyinfo.cpp
    typeinfo.cpp
)

add_library(KF5::FileMetaData ALIAS kfilemetadata)

target_link_libraries(kfilemetadata
  PUBLIC
    Qt5::Core
  PRIVATE
    KF5::Service
    KF5::I18n
)

generate_export_header(kfilemetadata BASE_NAME KFileMetaData EXPORT_FILE_NAME kfilemetadata_export.h)
set_target_properties(kfilemetadata PROPERTIES
    VERSION ${KFILEMETADATA_VERSION_STRING}
    SOVERSION ${KFILEMETADATA_SOVERSION}
    EXPORT_NAME FileMetaData
)
target_include_directories(kfilemetadata INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>")

install(TARGETS kfilemetadata ${INSTALL_TARGETS_DEFAULT_ARGS})

install(TARGETS kfilemetadata EXPORT KFileMetaDataTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kfilemetadataextractor.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install(EXPORT KFileMetaDataTargets
        NAMESPACE KF5::
        DESTINATION ${LIB_INSTALL_DIR}/cmake/KFileMetaData
        FILE KFileMetaDataTargetsWithPrefix.cmake)

install(FILES
    extractionresult.h
    extractorplugin.h
    extractorpluginmanager.h
    properties.h
    propertyinfo.h
    types.h
    typeinfo.h
    ${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_export.h

    DESTINATION ${INCLUDE_INSTALL_DIR}/kfilemetadata COMPONENT Devel
)

add_subdirectory(extractors)
