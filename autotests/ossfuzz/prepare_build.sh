# SPDX-FileCopyrightText: 2025 Azhar Momin <azhar.momin@kdemail.net>
# SPDX-License-Identifier: LGPL-2.0-or-later

apt-get update && \
    apt-get install -y cmake make autoconf automake libtool autopoint libmount-dev \
    wget po4a gettext pkg-config nasm python3 gperf

pip3 install meson ninja

git clone --depth 1 https://github.com/madler/zlib.git
git clone --depth=1 https://gitlab.freedesktop.org/freetype/freetype.git
wget https://sourceforge.net/projects/libpng/files/libpng16/1.6.50/libpng-1.6.50.tar.gz
git clone --depth=1 https://gitlab.freedesktop.org/fontconfig/fontconfig.git
git clone --depth 1 -b dev git://code.qt.io/qt/qtbase.git
git clone --depth 1 https://invent.kde.org/frameworks/extra-cmake-modules.git
git clone --depth 1 https://invent.kde.org/frameworks/karchive.git
git clone --depth 1 https://invent.kde.org/frameworks/kcoreaddons.git
git clone --depth 1 https://invent.kde.org/frameworks/kconfig.git
git clone --depth 1 https://invent.kde.org/frameworks/ki18n.git
git clone --depth 1 https://invent.kde.org/frameworks/kcodecs.git
git clone --depth 1 https://gitlab.freedesktop.org/poppler/poppler.git
wget https://github.com/nemtrif/utfcpp/archive/refs/tags/v4.0.6.tar.gz -O utfcpp-4.0.6.tar.gz
git clone --depth 1 https://github.com/taglib/taglib.git
git clone --depth 1 https://github.com/google/brotli.git
git clone --depth 1 https://github.com/libexpat/libexpat.git
git clone --depth 1 https://github.com/fmtlib/fmt.git
git clone --depth 1 https://github.com/exiv2/exiv2.git
git clone --depth 1 https://git.ffmpeg.org/ffmpeg.git
git clone --depth 1 https://gitlab.gnome.org/GNOME/libxml2.git
git clone --depth 1 https://github.com/nih-at/libzip.git
wget https://sourceforge.net/projects/ebook-tools/files/ebook-tools/0.2.2/ebook-tools-0.2.2.tar.gz
git clone http://www.wagner.pp.ru/git/oss/catdoc.git
wget https://download-mirror.savannah.gnu.org/releases/attr/attr-2.5.2.tar.gz
git clone --depth 1 https://invent.kde.org/graphics/kdegraphics-mobipocket.git
