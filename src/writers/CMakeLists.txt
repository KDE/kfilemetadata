if(Taglib_FOUND)
    add_library(kfilemetadata_taglibwriter MODULE taglibwriter.cpp ${debug_SRCS})

    target_link_libraries( kfilemetadata_taglibwriter
        KF5::FileMetaData
        Taglib::Taglib
    )

    set_target_properties(kfilemetadata_taglibwriter PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/kf${QT_MAJOR_VERSION}/kfilemetadata/writers")
    install(
    TARGETS kfilemetadata_taglibwriter
    DESTINATION ${KDE_INSTALL_PLUGINDIR}/kf${QT_MAJOR_VERSION}/kfilemetadata/writers)

endif()
