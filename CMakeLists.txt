cmake_minimum_required(VERSION 2.8.12)

project(kfilemetadata)

include(FeatureSummary)
find_package(ECM 5.18.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)
include(ECMAddTests)
include(GenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)

set(KF5_VERSION "5.19.0") # handled by release scripts
set(KF5_DEP_VERSION "5.18.0") # handled by release scripts

ecm_setup_version(${KF5_VERSION}
                  VARIABLE_PREFIX KFILEMETADATA
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5FileMetaDataConfigVersion.cmake"
                  SOVERSION 3)

# Dependencies
set(REQUIRED_QT_VERSION 5.3.0)

find_package(Qt5 ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Xml)
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS I18n Archive)

find_package(PopplerQt5 0.12.1)
set_package_properties(PopplerQt5 PROPERTIES DESCRIPTION "A PDF rendering library"
                       URL "http://poppler.freedesktop.org" TYPE OPTIONAL
                       PURPOSE "Support for PDF files")

find_package(Taglib 1.9)
set_package_properties(Taglib PROPERTIES DESCRIPTION "Id3 tag reader"
                       URL "http://developer.kde.org/~wheeler/taglib.html" TYPE OPTIONAL
                       PURPOSE "Support for music metadata")

find_package(Exiv2 0.21)
set_package_properties(Exiv2 PROPERTIES DESCRIPTION "Image Tag reader"
                       URL "http://www.exiv2.org" TYPE OPTIONAL
                       PURPOSE "Support for image metadata")

find_package(FFmpeg 1.0)
set_package_properties(FFmpeg PROPERTIES DESCRIPTION "Video Tag reader"
                       URL "http://ffmpeg.org" TYPE OPTIONAL
                       PURPOSE "Support for video metadata")

find_package(EPub)
set_package_properties(EPub PROPERTIES DESCRIPTION "Ebook epub reader"
                       URL "http://sourceforge.net/projects/ebook-tools" TYPE OPTIONAL
                       PURPOSE "Support for epub metadata")

find_package(CatDoc)
set_package_properties(CatDoc PROPERTIES DESCRIPTION "catdoc executable"
                       URL "http://www.wagner.pp.ru/~vitus/software/catdoc/" TYPE OPTIONAL
                       PURPOSE "Extract text from office 98 files - RUNTIME dependency")

#find_package(QMobipocket)
#set_package_properties(QMobipocket PROPERTIES DESCRIPTION "Mobipocket epub reader"
#                       URL "https://projects.kde.org/projects/kde/kdegraphics/kdegraphics-mobipocket"
#                       TYPE OPTIONAL PURPOSE "Support for mobi metadata")

add_definitions(-DTRANSLATION_DOMAIN=\"kfilemetadata\")

add_subdirectory(src)
if (BUILD_TESTING)
  add_subdirectory(autotests)
  add_subdirectory(tests)
endif()

# Config files
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5FileMetaData")

if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install(po)
endif()

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/KF5FileMetaDataConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5FileMetaDataConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/KF5FileMetaDataConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/KF5FileMetaDataConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT devel
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
