#!/bin/bash -eu

# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: LGPL-2.0-or-later

LDFLAGS=""
if [[ $FUZZING_ENGINE == "afl" ]]; then
    LDFLAGS="-fuse-ld=lld"
fi
export LDFLAGS

rm -rf $WORK/*

export PATH="$WORK/bin:$WORK/libexec:$PATH"
export PKG_CONFIG="$(which pkg-config) --static"
export PKG_CONFIG_PATH="$WORK/lib/pkgconfig:$WORK/lib/x86_64-linux-gnu/pkgconfig"
export CMAKE_PREFIX_PATH="$WORK"

# Build qtbase and dependencies
cd $SRC/zlib
./configure \
    --static --prefix $WORK
make install -j$(nproc)

cd $SRC/freetype
./autogen.sh
./configure \
    --enable-static \
    --disable-shared \
    --prefix $WORK
make install -j$(nproc)

cd $SRC/libexpat/expat
cmake . -G Ninja \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DBUILD_SHARED_LIBS=OFF \
  -DEXPAT_BUILD_TESTS=OFF \
  -DEXPAT_BUILD_EXAMPLES=OFF
ninja install -j$(nproc)

cd $SRC/
tar xzf libpng-*.tar.gz && rm -f libpng-*.tar.gz
cd libpng-*
cmake . -G Ninja \
    -DCMAKE_INSTALL_PREFIX=$WORK \
    -DPNG_SHARED=OFF \
    -DPNG_STATIC=ON \
    -DPNG_TESTS=OFF \
    -DPNG_TOOLS=OFF
ninja install -j$(nproc)

cd $SRC/fontconfig
meson setup builddir \
  --prefix=$WORK \
  --default-library=static \
  --buildtype=plain \
  -Dtests=disabled \
  -Dtools=disabled
ninja install -C builddir -j$(nproc)

# A workaround that lets QtGui find all transitive dependencies of Fontconfig correctly
sed -i 's|INTERFACE_INCLUDE_DIRECTORIES "${Fontconfig_INCLUDE_DIR}"|INTERFACE_INCLUDE_DIRECTORIES "${Fontconfig_INCLUDE_DIR}"\nINTERFACE_LINK_LIBRARIES "${PKG_FONTCONFIG_LINK_LIBRARIES}"|' \
    /usr/local/share/cmake-3.29/Modules/FindFontconfig.cmake

cd $SRC/qtbase
./configure -platform linux-clang-libc++ -prefix $WORK \
  -qpa minimal -default-qpa minimal \
  -opensource -confirm-license -debug \
  -qt-pcre \
  -static -no-glib -no-icu -no-opengl -no-feature-sql -no-feature-network \
  -no-feature-dbus -no-feature-printsupport
ninja install -j$(nproc)

# Build ECM
cd $SRC/extra-cmake-modules
cmake . -G Ninja \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -j$(nproc)

# Build karchive
cd $SRC/karchive
rm -r poqm
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DWITH_BZIP2=OFF \
  -DWITH_LIBLZMA=OFF \
  -DWITH_OPENSSL=OFF \
  -DWITH_LIBZSTD=OFF
ninja install -j$(nproc)

# Build kcoreaddons
cd $SRC/kcoreaddons
rm -r poqm
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DBUILD_PYTHON_BINDINGS=OFF \
  -DKCOREADDONS_USE_QML=OFF \
  -DUSE_DBUS=OFF
ninja install -j$(nproc)

# Build kconfig
cd $SRC/kconfig
rm -r poqm
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DKCONFIG_USE_GUI=OFF \
  -DKCONFIG_USE_QML=OFF \
  -DUSE_DBUS=OFF
ninja install -j$(nproc)

# Build ki18n
cd $SRC/ki18n
rm -r po
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DBUILD_WITH_QML=OFF
ninja install -j$(nproc)

# Build kcodecs
cd $SRC/kcodecs
rm -r poqm
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DBUILD_TESTING=OFF
ninja install -j$(nproc)

# Build poppler
cd $SRC/poppler
cmake . -G Ninja \
  -DCMAKE_CXX_SCAN_FOR_MODULES=OFF \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DBUILD_GTK_TESTS=OFF \
  -DBUILD_QT5_TESTS=OFF \
  -DBUILD_QT6_TESTS=OFF \
  -DBUILD_CPP_TESTS=OFF \
  -DBUILD_MANUAL_TESTS=OFF \
  -DENABLE_BOOST=OFF \
  -DENABLE_UTILS=OFF \
  -DENABLE_CPP=OFF \
  -DENABLE_GLIB=OFF \
  -DENABLE_GOBJECT_INTROSPECTION=OFF \
  -DENABLE_GTK_DOC=OFF \
  -DENABLE_QT5=OFF \
  -DENABLE_QT6=ON \
  -DENABLE_LIBOPENJPEG="none" \
  -DENABLE_DCTDECODER="none" \
  -DENABLE_LCMS=OFF \
  -DENABLE_LIBCURL=OFF \
  -DENABLE_LIBTIFF=OFF \
  -DENABLE_LIBJPEG=OFF \
  -DENABLE_LIBOPENJPEG=OFF \
  -DENABLE_NSS3=OFF \
  -DENABLE_GPGME=OFF
ninja install -j$(nproc)

# Build taglib and its dependencies
cd $SRC
tar -xzf utfcpp-*.tar.gz && rm -f utfcpp-*.tar.gz
cp -r utfcpp-*/source/* $WORK/include/

cd $SRC/taglib
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -j$(nproc)

# Build exiv2 and its dependencies
cd $SRC/brotli
cmake -B builddir -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -C builddir -j$(nproc)

cd $SRC/libexpat/expat
cmake -B builddir -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DEXPAT_BUILD_TESTS=OFF \
  -DEXPAT_BUILD_EXAMPLES=OFF
ninja install -C builddir -j$(nproc)

cd $SRC/fmt
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DFMT_DOC=OFF \
  -DFMT_TEST=OFF
ninja install -j$(nproc)

cd $SRC/exiv2
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK \
  -DEXIV2_ENABLE_DYNAMIC_RUNTIME=OFF \
  -DEXIV2_BUILD_EXIV2_COMMAND=OFF \
  -DEXIV2_ENABLE_INIH=OFF
ninja install -j$(nproc)

# Build ffmpeg (avcodec, avformat, avutil)
cd $SRC/ffmpeg
if [ "$SANITIZER" = "memory" ]; then
  disable_asm="--disable-inline-asm --disable-x86asm"
else
  disable_asm=
fi
./configure \
  --cc="$CC" \
  --cxx="$CXX" \
  --ld="$CXX $CXXFLAGS -std=c++11" \
  --prefix=$WORK \
  $disable_asm \
  --enable-static \
  --disable-shared \
  --disable-doc \
  --disable-everything \
  --disable-programs \
  --disable-avdevice \
  --disable-avfilter \
  --disable-swresample \
  --disable-swscale \
  --enable-avcodec \
  --enable-avformat \
  --enable-avutil
make install -j$(nproc)

# Build libepub and its dependencies
cd $SRC/libxml2
cmake -B builddir -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -C builddir -j$(nproc)

cd $SRC/libzip
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -j$(nproc)

cd $SRC
tar -xzf ebook-tools-*.tar.gz && rm -f ebook-tools-*.tar.gz
cd ebook-tools-*
# disable the cli tools
sed -i '/add_subdirectory *(tools)/d' src/CMakeLists.txt
sed -i 's/add_library *(epub SHARED/add_library(epub/' src/libepub/CMakeLists.txt
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -j$(nproc)

# Build catdoc
CFLAGS_SAVE="$CFLAGS"
CXXFLAGS_SAVE="$CXXFLAGS"
unset CFLAGS
unset CXXFLAGS
export AFL_NOOPT=1
cd $SRC/catdoc
mkdir -p $WORK/share/catdoc # not sure why it fails without this
./configure --disable-charset-check --disable-wordview --disable-langinfo --prefix $WORK
make install -j$(nproc)
cp $WORK/bin/{catdoc,xls2csv,catppt} $OUT
unset AFL_NOOPT
export CFLAGS="$CFLAGS_SAVE"
export CXXFLAGS="$CXXFLAGS_SAVE"

# Build attr
cd $SRC
tar -xzf attr-*.tar.gz && rm -f attr-*.tar.gz
cd attr-*
./configure --enable-static --disable-shared --prefix $WORK
make install -j$(nproc)

# Build kdegraphics-mobipocket
cd $SRC/kdegraphics-mobipocket
sed -i '/add_subdirectory(tools)/d' CMakeLists.txt
cmake . -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja install -j$(nproc)

# Build the fuzzers
cd $SRC/kfilemetadata
cmake -B build -G Ninja \
  -DBUILD_SHARED_LIBS=OFF \
  -DBUILD_TESTING=OFF \
  -DBUILD_FUZZERS=ON \
  -DFUZZERS_USE_QT_MINIMAL_INTEGRATION_PLUGIN=ON \
  -DCMAKE_INSTALL_PREFIX=$WORK
ninja -C build -j$(nproc)

EXTENSIONS="epubextractor_fuzzer epub
            exiv2extractor_fuzzer heif webp jpg avif png jxl
            fb2extractor_fuzzer fb2 fb2.zip
            ffmpegextractor_fuzzer ogv mkv webm ts avi
            krita_fuzzer kra
            mobiextractor_fuzzer mobi
            odfextractor_fuzzer odp fodp odg fodg odt fodt ods
            office2007extractor_fuzzer docx dotx xlsx xltx pptx potx ppsx sldx xps oxps 3mf
            officeextractor_fuzzer doc dot xls xlt xla ppt pot pps
            plaintextextractor_fuzzer txt pl html
            pngextractor_fuzzer png
            poextractor_fuzzer po
            popplerextractor_fuzzer pdf
            postscriptdscextractor_fuzzer ps eps
            taglibextractor_fuzzer flac mp3 m4a ogg wav aif ape wma mpc opus spx wv mod s3m xm it aax aaxc
            xmlextractor_fuzzer xml svg svgz"

echo "$EXTENSIONS" | while read fuzzer_name extensions; do
    # copy the fuzzer binary
    cp build/bin/fuzzers/$fuzzer_name $OUT

    # create seed corpus
    for extension in $extensions; do
        find . -name "*.$extension" -exec zip -q "$OUT/${fuzzer_name}_seed_corpus.zip" {} +
    done
done
