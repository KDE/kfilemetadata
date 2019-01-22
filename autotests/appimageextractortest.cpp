/*
 * Copyright (C) 2019  Friedrich W. H. Kossebau <kossebau@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "appimageextractortest.h"

// Qt
#include <QTest>
#include <QDebug>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/appimageextractor.h"

using namespace KFileMetaData;

QString AppImageExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

/*
The test.AppImage got created by:

wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-x86_64.AppImage
./linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage --executable=/usr/bin/echo --desktop-file=appimagetest.desktop --icon-file=/usr/share/icons/oxygen/base/16x16/apps/kde.png
mkdir AppDir/usr/share/metainfo/
cp org.kde.kfilemetadata.appimagetest.appdata.xml AppDir/usr/share/metainfo/
./linuxdeploy-x86_64.AppImage --appdir AppDir --output appimage
mv Test_Desktop_Name-x86_64.AppImage test.AppImage

To edit the appdata.xml & desktop file, run ./test.AppImage --appimage-extract to get the files.
Use linuxdeploy to create the appimage then again.
*/

void AppImageExtractorTest::test()
{
    AppImageExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.AppImage")), QStringLiteral("application/vnd.appimage"));
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 0);

    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Konqi")));
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("Test Desktop Name")));
    QCOMPARE(result.properties().value(Property::Comment), QVariant(QStringLiteral("Test Desktop Comment")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("Test description line 1.\nTest description line 2.")));
    QCOMPARE(result.properties().value(Property::License), QVariant(QStringLiteral("GPL-2.0")));

}

QTEST_GUILESS_MAIN(AppImageExtractorTest)
