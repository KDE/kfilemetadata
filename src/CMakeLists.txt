add_library(kfilemetadata SHARED
    extractionresult.cpp
    extractorplugin.cpp
    extractorpluginmanager.cpp
)

target_link_libraries(kfilemetadata
  PUBLIC
    Qt5::Core
  PRIVATE
    KF5::KDE4Support
    KF5::Service
    Qt5::Widgets
)

install(TARGETS kfilemetadata ${INSTALL_TARGETS_DEFAULT_ARGS})

install(TARGETS kfilemetadata EXPORT KdeLibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kfilemetadataextractor.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install(EXPORT KdeLibsLibraryTargets
        DESTINATION ${LIB_INSTALL_DIR}/cmake/KFileMetaData
        FILE KFileMetaDataTargetsWithPrefix.cmake)


install(FILES
    extractionresult.h
    extractorplugin.h
    extractorpluginmanager.h
    kfilemetadata_export.h

    DESTINATION ${INCLUDE_INSTALL_DIR}/kfilemetadata COMPONENT Devel
)

add_subdirectory(extractors)
#add_subdirectory(test)
