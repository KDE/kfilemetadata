# - Try to find the Exiv2 library
#
#  EXIV2_MIN_VERSION - You can set this variable to the minimum version you need
#                      before doing FIND_PACKAGE(Exiv2). The default is 0.12.
#
# Once done this will define
#
#  EXIV2_FOUND - system has libexiv2
#  EXIV2_INCLUDE_DIR - the libexiv2 include directory
#  EXIV2_LIBRARIES - Link these to use libexiv2
#  EXIV2_DEFINITIONS - Compiler switches required for using libexiv2
#
# The minimum required version of Exiv2 can be specified using the
# standard syntax, e.g. find_package(Exiv2 0.17)
#
# For compatibility, also the variable EXIV2_MIN_VERSION can be set to the minimum version
# you need before doing FIND_PACKAGE(Exiv2). The default is 0.12.

# Copyright (c) 2010, Alexander Neundorf, <neundorf@kde.org>
# Copyright (c) 2008, Gilles Caulier, <caulier.gilles@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.

# Support EXIV2_MIN_VERSION for compatibility:
if(NOT Exiv2_FIND_VERSION)
  set(Exiv2_FIND_VERSION "${EXIV2_MIN_VERSION}")
endif(NOT Exiv2_FIND_VERSION)

# the minimum version of exiv2 we require
if(NOT Exiv2_FIND_VERSION)
  set(Exiv2_FIND_VERSION "0.12")
endif(NOT Exiv2_FIND_VERSION)


if (NOT WIN32)
   # use pkg-config to get the directories and then use these values
   # in the FIND_PATH() and FIND_LIBRARY() calls
   find_package(PkgConfig)
   pkg_check_modules(PC_EXIV2 QUIET exiv2)
   set(EXIV2_DEFINITIONS ${PC_EXIV2_CFLAGS_OTHER})
endif (NOT WIN32)


find_path(EXIV2_INCLUDE_DIR NAMES exiv2/exif.hpp
          HINTS
          ${PC_EXIV2_INCLUDEDIR}
          ${PC_EXIV2_INCLUDE_DIRS}
        )

find_library(EXIV2_LIBRARY NAMES exiv2 libexiv2
             HINTS
             ${PC_EXIV2_LIBDIR}
             ${PC_EXIV2_LIBRARY_DIRS}
            )


# Get the version number from exiv2/version.hpp and store it in the cache:
if(EXIV2_INCLUDE_DIR  AND NOT  EXIV2_VERSION)
  file(READ ${EXIV2_INCLUDE_DIR}/exiv2/version.hpp EXIV2_VERSION_CONTENT)
  string(REGEX MATCH "#define EXIV2_MAJOR_VERSION +\\( *([0-9]+) *\\)"  _dummy "${EXIV2_VERSION_CONTENT}")
  set(EXIV2_VERSION_MAJOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define EXIV2_MINOR_VERSION +\\( *([0-9]+) *\\)"  _dummy "${EXIV2_VERSION_CONTENT}")
  set(EXIV2_VERSION_MINOR "${CMAKE_MATCH_1}")

  string(REGEX MATCH "#define EXIV2_PATCH_VERSION +\\( *([0-9]+) *\\)"  _dummy "${EXIV2_VERSION_CONTENT}")
  set(EXIV2_VERSION_PATCH "${CMAKE_MATCH_1}")

  set(EXIV2_VERSION "${EXIV2_VERSION_MAJOR}.${EXIV2_VERSION_MINOR}.${EXIV2_VERSION_PATCH}" CACHE STRING "Version number of Exiv2" FORCE)
endif(EXIV2_INCLUDE_DIR  AND NOT  EXIV2_VERSION)

set(EXIV2_LIBRARIES "${EXIV2_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Exiv2  REQUIRED_VARS  EXIV2_LIBRARY EXIV2_INCLUDE_DIR
                                         VERSION_VAR  EXIV2_VERSION)

mark_as_advanced(EXIV2_INCLUDE_DIR EXIV2_LIBRARY)

