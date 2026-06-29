# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: BSD-2-Clause

if(DEFINED ENV{LIB_FUZZING_ENGINE})
    set(fuzzing_engine $ENV{LIB_FUZZING_ENGINE})
else()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(fuzzing_engine -fsanitize=fuzzer)
    else()
        message(FATAL_ERROR "Fuzzing engine not supported")
    endif()
endif()

function(add_extractor_fuzzer extractor extractor_header target_lib)
    set(gen_src ${CMAKE_CURRENT_BINARY_DIR}/${target_lib}_fuzzer.cc)
    set(EXTRACTOR "${extractor}")
    set(EXTRACTOR_HEADER "${extractor_header}")
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/kfilemetadata_fuzzer.cc.in
        ${gen_src}
        @ONLY
    )

    add_executable(${target_lib}_fuzzer
        ${gen_src}
    )

    target_link_libraries(${target_lib}_fuzzer
        PRIVATE
            Qt6::Gui
            kfilemetadata_${target_lib}
            ${fuzzing_engine}
    )

    if(FUZZERS_USE_QT_MINIMAL_INTEGRATION_PLUGIN)
        qt_import_plugins(${target_lib}_fuzzer INCLUDE Qt::QMinimalIntegrationPlugin)
    endif()

    set_target_properties(${target_lib}_fuzzer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/fuzzers
    )
endfunction()

if(TARGET Poppler::Qt6)
    add_extractor_fuzzer(PopplerExtractor popplerextractor popplerextractor)
endif()

if(Taglib_FOUND)
    add_extractor_fuzzer(TagLibExtractor taglibextractor taglibextractor)
endif()

if(LibExiv2_FOUND)
    add_extractor_fuzzer(Exiv2Extractor exiv2extractor exiv2extractor)
endif()

if(AVCODEC_FOUND AND AVFORMAT_FOUND AND AVUTIL_FOUND)
    add_extractor_fuzzer(FFmpegExtractor ffmpegextractor ffmpegextractor)
endif()

if(EPUB_FOUND)
    add_extractor_fuzzer(EPubExtractor epubextractor epubextractor)
endif()

add_extractor_fuzzer(PlainTextExtractor plaintextextractor plaintextextractor)

add_extractor_fuzzer(POExtractor poextractor poextractor)

add_extractor_fuzzer(XmlExtractor xmlextractor xmlextractor)

add_extractor_fuzzer(DscExtractor postscriptdscextractor postscriptdscextractor)

if(KF6Archive_FOUND)
    add_extractor_fuzzer(OdfExtractor odfextractor odfextractor)

    add_extractor_fuzzer(Office2007Extractor office2007extractor office2007extractor)
endif()

add_extractor_fuzzer(OfficeExtractor officeextractor officeextractor)

if (QMobipocket6_FOUND)
    add_extractor_fuzzer(MobiExtractor mobiextractor mobiextractor)
endif()

if(libappimage_FOUND AND KF6Config_FOUND)
    add_extractor_fuzzer(AppImageExtractor appimageextractor appimageextractor)
endif()

add_extractor_fuzzer(PngExtractor pngextractor pngextractor)

if(KF6Archive_FOUND)
    add_extractor_fuzzer(Fb2Extractor fb2extractor fb2extractor)

    add_extractor_fuzzer(KritaExtractor kritaextractor krita)
endif()
