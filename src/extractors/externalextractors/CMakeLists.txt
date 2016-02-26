install(
    DIRECTORY pdfextractor
    DESTINATION ${KF5_LIBEXEC_INSTALL_DIR}/kfilemetadata/externalextractors
    PATTERN "*.py"
    PERMISSIONS WORLD_READ OWNER_WRITE WORLD_EXECUTE
    )
