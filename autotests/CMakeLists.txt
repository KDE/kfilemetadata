configure_file(indexerextractortestsconfig.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/indexerextractortestsconfig.h @ONLY)

kde4_add_unit_test(extractortests
  indexerextractortests.cpp
  simpleresult.cpp
  ../src/extractors/plaintextextractor.cpp
)

target_link_libraries(extractortests
  ${QT_QTTEST_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  kfilemetadata
)
