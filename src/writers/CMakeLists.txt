if(TAGLIB_FOUND)
    include_directories( ${TAGLIB_INCLUDES} )

    add_library(kfilemetadata_taglibwriter MODULE taglibwriter.cpp)

    target_link_libraries( kfilemetadata_taglibwriter
        KF5::FileMetaData
        ${TAGLIB_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_taglibwriter
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata/writers)

endif()
