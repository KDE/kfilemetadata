cmake_minimum_required(VERSION 3.16)

set(KF_VERSION "5.103.0") # handled by release scripts
set(KF_DEP_VERSION "5.103.0") # handled by release scripts
project(KFileMetaData VERSION ${KF_VERSION})

include(FeatureSummary)
find_package(ECM 5.103.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://commits.kde.org/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)
include(ECMAddTests)
include(ECMGenerateExportHeader)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMAddQch)
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)
include(CheckStructHasMember)

set(EXCLUDE_DEPRECATED_BEFORE_AND_AT 0 CACHE STRING "Control the range of deprecated API excluded from the build [default=0].")

option(BUILD_QCH "Build API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)" OFF)
add_feature_info(QCH ${BUILD_QCH} "API documentation in QCH format (for e.g. Qt Assistant, Qt Creator & KDevelop)")

ecm_setup_version(PROJECT
                  VARIABLE_PREFIX KFILEMETADATA
                  VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_version.h"
                  PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5FileMetaDataConfigVersion.cmake"
                  SOVERSION 3)

# Dependencies
set(REQUIRED_QT_VERSION 5.15.2)

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} CONFIG COMPONENTS Gui)
set_package_properties(Qt${QT_MAJOR_VERSION}Gui PROPERTIES DESCRIPTION "Qt${QT_MAJOR_VERSION}: Gui component"
                       TYPE OPTIONAL
                       PURPOSE "Qt${QT_MAJOR_VERSION}::Gui is needed to build the AppImage extractor")

find_package(Qt${QT_MAJOR_VERSION} ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE COMPONENTS Xml)
if (QT_MAJOR_VERSION STREQUAL "6")
    find_package(Qt6Core5Compat)
endif()

find_package(KF5 ${KF_DEP_VERSION} COMPONENTS Archive)
set_package_properties(KF5Archive PROPERTIES DESCRIPTION "KDE Frameworks 5: Archive Framework"
                       URL "https://download.kde.org/stable/frameworks/" TYPE OPTIONAL
                       PURPOSE "Archive is needed to build ODF and OOXML 2007 extractors")

find_package(KF5 ${KF_DEP_VERSION} COMPONENTS CoreAddons)
set_package_properties(KF5CoreAddons PROPERTIES DESCRIPTION "KDE Frameworks 5: Core Addons Framework"
                       URL "https://download.kde.org/stable/frameworks/" TYPE REQUIRED
                       PURPOSE "Needed for the formatting of properties for display purposes")

find_package(KF5 ${KF_DEP_VERSION} COMPONENTS Config)
set_package_properties(KF5Config PROPERTIES DESCRIPTION "KDE Frameworks 5: Config Framework"
                       URL "https://download.kde.org/stable/frameworks/" TYPE OPTIONAL
                       PURPOSE "Config is needed to build the AppImage extractor")

find_package(KF5 ${KF_DEP_VERSION} REQUIRED COMPONENTS I18n)

find_package(Poppler 0.12.1 COMPONENTS Qt${QT_MAJOR_VERSION})
set_package_properties(Poppler PROPERTIES DESCRIPTION "A PDF rendering library"
                       URL "https://poppler.freedesktop.org/" TYPE OPTIONAL
                       PURPOSE "Support for PDF files")

find_package(Taglib 1.11.1)
set_package_properties(Taglib PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Support for music metadata")

find_package(LibExiv2 0.21)
set_package_properties(LibExiv2 PROPERTIES
                       TYPE OPTIONAL
                       PURPOSE "Support for image metadata")

find_package(FFmpeg 57.48 COMPONENTS AVCODEC)
find_package(FFmpeg 57.40 COMPONENTS AVFORMAT)
find_package(FFmpeg 55.27 COMPONENTS AVUTIL)
set_package_properties(FFmpeg PROPERTIES DESCRIPTION "Video Tag reader"
                       URL "https://ffmpeg.org/" TYPE OPTIONAL
                       PURPOSE "Support for video metadata")

find_package(EPub)
set_package_properties(EPub PROPERTIES DESCRIPTION "Ebook epub reader"
                       URL "https://sourceforge.net/projects/ebook-tools/" TYPE OPTIONAL
                       PURPOSE "Support for epub metadata")

find_package(CatDoc)
set_package_properties(CatDoc PROPERTIES DESCRIPTION "catdoc executable"
                       URL "https://www.wagner.pp.ru/~vitus/software/catdoc/" TYPE RUNTIME
                       PURPOSE "Extract text from office 98 files - RUNTIME dependency")

if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
    find_package(Xattr)
    set_package_properties(Xattr PROPERTIES DESCRIPTION "library libattr "
        URL "https://savannah.nongnu.org/projects/attr"
        TYPE REQUIRED
        PURPOSE "Extended attribute shared library")
endif()

find_package(QMobipocket 2.0)
set_package_properties(QMobipocket PROPERTIES DESCRIPTION "Mobipocket epub reader"
                       URL "https://invent.kde.org/graphics/kdegraphics-mobipocket"
                       TYPE OPTIONAL PURPOSE "Support for mobi metadata")

find_package(libappimage 0.1.10 CONFIG)
set_package_properties(libappimage PROPERTIES DESCRIPTION "Core library of the AppImage project"
                       URL "https://github.com/AppImage/libappimage"
                       TYPE OPTIONAL
                       PURPOSE "Needed to build the AppImage extractor"
                       )

ecm_set_disabled_deprecation_versions(
    QT 5.14.0 # TODO look into the QMap::insertMulti usage
    KF 5.97
)

add_definitions(-DTRANSLATION_DOMAIN=\"kfilemetadata5\")
add_subdirectory(src)
if (BUILD_TESTING)
  add_subdirectory(autotests)
  add_subdirectory(tests)
endif()

# Config files
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5FileMetaData")

ki18n_install(po)

if (BUILD_QCH)
    ecm_install_qch_export(
        TARGETS KF5FileMetaData_QCH
        FILE KF5FileMetaDataQchTargets.cmake
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel
    )
    set(PACKAGE_INCLUDE_QCHTARGETS "include(\"\${CMAKE_CURRENT_LIST_DIR}/KF5FileMetaDataQchTargets.cmake\")")
endif()

include(CMakePackageConfigHelpers)

configure_package_config_file(
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

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/kfilemetadata_version.h
  DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KFileMetaData COMPONENT Devel
)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
