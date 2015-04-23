# - Find EPub
# Find the EPub library.
#
# This module defines
#  EPUB_FOUND - whether the EPub library was found
#  EPUB_LIBRARIES - the EPub library
#  EPUB_INCLUDE_DIR - the include path of the EPub library

# Copyright (c) 2008, Pino Toscano, <pino@kde.org>
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


if (EPUB_INCLUDE_DIR AND EPUB_LIBRARIES)

  # Already in cache
  set (EPUB_FOUND TRUE)

else (EPUB_INCLUDE_DIR AND EPUB_LIBRARIES)

  find_library (EPUB_LIBRARIES
    NAMES epub libepub
  )

  find_path (EPUB_INCLUDE_DIR
    NAMES epub.h
  )

  include (FindPackageHandleStandardArgs)
  find_package_handle_standard_args (EPub DEFAULT_MSG EPUB_LIBRARIES EPUB_INCLUDE_DIR)

endif (EPUB_INCLUDE_DIR AND EPUB_LIBRARIES)

mark_as_advanced(EPUB_INCLUDE_DIR EPUB_LIBRARIES)
