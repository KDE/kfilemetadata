project(kfilemetadata)
cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 0.0.9 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package(Qt5 REQUIRED NO_MODULE COMPONENTS Widgets Xml)
find_package(KF5 REQUIRED NO_MODULE COMPONENTS KDE4Support I18n Service)

include(KDEInstallDirs)
include(KDECMakeSettings)

find_package(PopplerQt5 0.12.1)
set_package_properties(PopplerQt5 PROPERTIES DESCRIPTION "A PDF rendering library"
                       URL "http://poppler.freedesktop.org" TYPE OPTIONAL
                       PURPOSE "Support for PDF files")

find_package(Taglib)
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

#find_package(QMobipocket)
#set_package_properties(QMobipocket PROPERTIES DESCRIPTION "Mobipocket epub reader"
#                       URL "https://projects.kde.org/projects/kde/kdegraphics/kdegraphics-mobipocket"
#                       TYPE OPTIONAL PURPOSE "Support for mobi metadata")

include_directories(
  ${CMAKE_SOURCE_DIR}/src
)

add_subdirectory(src)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/KFileMetaDataConfig.cmake.in
               ${CMAKE_CURRENT_BINARY_DIR}/KFileMetaDataConfig.cmake @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/KFileMetaDataConfig.cmake
        DESTINATION ${LIB_INSTALL_DIR}/cmake/KFileMetaData)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

