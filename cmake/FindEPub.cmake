# - Find EPub
# Find the EPub library.
#
# This module defines
#  EPUB_FOUND - whether the EPub library was found
#  EPUB_LIBRARY - the EPub library
#  EPUB_INCLUDE_DIR - the include path of the EPub library
#  EPub::EPub - the EPub target

# SPDX-FileCopyrightText: 2008 Pino Toscano <pino@kde.org>
# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: BSD-3-Clause

find_path(EPUB_INCLUDE_DIR
  NAMES epub.h
)

find_library(EPUB_LIBRARY
  NAMES epub libepub
)

find_package(LibZip QUIET)
find_package(LibXml2 QUIET)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EPub
  REQUIRED_VARS EPUB_LIBRARY EPUB_INCLUDE_DIR LibZip_FOUND LibXml2_FOUND
  VERSION_VAR EPUB_VERSION
)

if (EPUB_FOUND AND NOT TARGET EPub::EPub)
  add_library(EPub::EPub UNKNOWN IMPORTED)
  set_target_properties(EPub::EPub PROPERTIES
      IMPORTED_LOCATION "${EPUB_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${EPUB_INCLUDE_DIR}"
      INTERFACE_LINK_LIBRARIES "\$<LINK_ONLY:libzip::zip>;\$<LINK_ONLY:LibXml2::LibXml2>"
  )
endif()

mark_as_advanced(EPUB_INCLUDE_DIR EPUB_LIBRARY)
