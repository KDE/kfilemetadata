#
#  FIND_LIBRARY_WITH_DEBUG
#  -> enhanced FIND_LIBRARY to allow the search for an
#     optional debug library with a WIN32_DEBUG_POSTFIX similar
#     to CMAKE_DEBUG_POSTFIX when creating a shared lib
#     it has to be the second and third argument

# Copyright (c) 2007, Christian Ehrlicher, <ch.ehrlicher@gmx.de>

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

MACRO(FIND_LIBRARY_WITH_DEBUG var_name win32_dbg_postfix_name dgb_postfix libname)

  IF(NOT "${win32_dbg_postfix_name}" STREQUAL "WIN32_DEBUG_POSTFIX")

     # no WIN32_DEBUG_POSTFIX -> simply pass all arguments to FIND_LIBRARY
     FIND_LIBRARY(${var_name}
                  ${win32_dbg_postfix_name}
                  ${dgb_postfix}
                  ${libname}
                  ${ARGN}
     )

  ELSE(NOT "${win32_dbg_postfix_name}" STREQUAL "WIN32_DEBUG_POSTFIX")

    IF(NOT WIN32)
      # on non-win32 we don't need to take care about WIN32_DEBUG_POSTFIX

      FIND_LIBRARY(${var_name} ${libname} ${ARGN})

    ELSE(NOT WIN32)

      # 1. get all possible libnames
      SET(args ${ARGN})
      SET(newargs "")
      SET(libnames_release "")
      SET(libnames_debug "")

      LIST(LENGTH args listCount)

      IF("${libname}" STREQUAL "NAMES")
        SET(append_rest 0)
        LIST(APPEND args " ")

        FOREACH(i RANGE ${listCount})
          LIST(GET args ${i} val)

          IF(append_rest)
            LIST(APPEND newargs ${val})
          ELSE(append_rest)
            IF("${val}" STREQUAL "PATHS")
              LIST(APPEND newargs ${val})
              SET(append_rest 1)
            ELSE("${val}" STREQUAL "PATHS")
              LIST(APPEND libnames_release "${val}")
              LIST(APPEND libnames_debug   "${val}${dgb_postfix}")
            ENDIF("${val}" STREQUAL "PATHS")
          ENDIF(append_rest)

        ENDFOREACH(i)

      ELSE("${libname}" STREQUAL "NAMES")

        # just one name
        LIST(APPEND libnames_release "${libname}")
        LIST(APPEND libnames_debug   "${libname}${dgb_postfix}")

        SET(newargs ${args})

      ENDIF("${libname}" STREQUAL "NAMES")

      # search the release lib
      FIND_LIBRARY(${var_name}_RELEASE
                   NAMES ${libnames_release}
                   ${newargs}
      )

      # search the debug lib
      FIND_LIBRARY(${var_name}_DEBUG
                   NAMES ${libnames_debug}
                   ${newargs}
      )

      IF(${var_name}_RELEASE AND ${var_name}_DEBUG)

        # both libs found
        SET(${var_name} optimized ${${var_name}_RELEASE}
                        debug     ${${var_name}_DEBUG})

      ELSE(${var_name}_RELEASE AND ${var_name}_DEBUG)

        IF(${var_name}_RELEASE)

          # only release found
          SET(${var_name} ${${var_name}_RELEASE})

        ELSE(${var_name}_RELEASE)

          # only debug (or nothing) found
          SET(${var_name} ${${var_name}_DEBUG})

        ENDIF(${var_name}_RELEASE)
       
      ENDIF(${var_name}_RELEASE AND ${var_name}_DEBUG)

      MARK_AS_ADVANCED(${var_name}_RELEASE)
      MARK_AS_ADVANCED(${var_name}_DEBUG)

    ENDIF(NOT WIN32)

  ENDIF(NOT "${win32_dbg_postfix_name}" STREQUAL "WIN32_DEBUG_POSTFIX")

ENDMACRO(FIND_LIBRARY_WITH_DEBUG)
