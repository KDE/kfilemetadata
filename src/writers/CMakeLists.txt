if(Taglib_FOUND)
    add_library(kfilemetadata_taglibwriter MODULE taglibwriter.cpp ${debug_SRCS})

    target_link_libraries( kfilemetadata_taglibwriter
        KF5::FileMetaData
        Taglib::Taglib
    )

    set_target_properties(kfilemetadata_taglibwriter PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf5/kfilemetadata/writers")
    install(
    TARGETS kfilemetadata_taglibwriter
    DESTINATION ${PLUGIN_INSTALL_DIR}/kf5/kfilemetadata/writers)

endif()
