set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
  ${SOPRANO_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

configure_file(indexerextractortestsconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/indexerextractortestsconfig.h @ONLY)

kde4_add_unit_test(indexerextractortests
  indexerextractortests.cpp
  ../plaintextextractor.cpp)

target_link_libraries(indexerextractortests
  ${QT_QTTEST_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  nepomukextractor
  nepomukcommon
  nepomukcore
)
