if(TAGLIB_FOUND)
    add_library(kfilemetadata_taglibwriter MODULE taglibwriter.cpp)
    target_include_directories(kfilemetadata_taglibwriter SYSTEM PRIVATE ${TAGLIB_INCLUDES})

    target_link_libraries( kfilemetadata_taglibwriter
        KF5::FileMetaData
        ${TAGLIB_LIBRARIES}
    )

    install(
    TARGETS kfilemetadata_taglibwriter
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata/writers)

endif()
