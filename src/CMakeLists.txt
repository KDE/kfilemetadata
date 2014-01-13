kde4_add_library(kfilemetadata SHARED
    extractionresult.cpp
    extractorplugin.cpp
    extractorpluginmanager.cpp
)

target_link_libraries(kfilemetadata
  ${QT_QTCORE_LIBRARY}
  ${KDE4_KDECORE_LIBS}
)

set_target_properties(kfilemetadata PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION}
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
