# get from the full path to OkularConfig.cmake up to the base dir dir:
get_filename_component( _currentDir  ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component( _currentDir  ${_currentDir} PATH)
get_filename_component( _currentDir  ${_currentDir} PATH)
get_filename_component( _currentDir  ${_currentDir} PATH)


# find the full paths to the library and the includes:
find_path(KFILEMETADATA_INCLUDE_DIR kfilemetadata/kfilemetadata_export.h
          HINTS ${_currentDir}/include
          NO_DEFAULT_PATH)

find_library(KFILEMETADATA_LIBRARY kfilemetadata
             HINTS ${_currentDir}/lib
             NO_DEFAULT_PATH)

set(KFILEMETADATA_LIBRARIES ${KFILEMETADATA_LIBRARY})

if(KFILEMETADATA_INCLUDE_DIR AND KFILEMETADATA_LIBRARY)
    set(KFILEMETADATA_FOUND TRUE)
endif()

