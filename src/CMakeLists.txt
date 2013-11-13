kde4_add_library(kfilemetadata SHARED
    extractorplugin.cpp
    extractorpluginmanager.cpp
)

target_link_libraries(kfilemetadata
  ${QT_QTCORE_LIBRARY}
  ${KDE4_KDECORE_LIBS}
)

install(TARGETS kfilemetadata ${INSTALL_TARGETS_DEFAULT_ARGS})

install(TARGETS kfilemetadata EXPORT KdeLibsLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES kfilemetadataextractor.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

install(FILES
    extractorplugin.h
    extractorpluginmanager.h
    kfilemetadata_export.h

    DESTINATION ${INCLUDE_INSTALL_DIR}/kfilemetadata COMPONENT Devel
)

add_subdirectory(extractors)
#add_subdirectory(test)
