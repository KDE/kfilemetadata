/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
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

#include "exiv2extractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/exiv2extractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>

using namespace KFileMetaData;
using namespace KFileMetaData::Property;

QString Exiv2ExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void Exiv2ExtractorTest::test()
{
    Exiv2Extractor plugin{this};

    SimpleExtractionResult result(testFilePath("test.jpg"), "image/jpeg");
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    QCOMPARE(result.properties().value(Artist).toString(), QStringLiteral("Artist"));
    QCOMPARE(result.properties().value(Description).toString(), QStringLiteral("Description"));
    QCOMPARE(result.properties().value(Copyright).toString(), QStringLiteral("Copyright"));
    QCOMPARE(result.properties().value(Generator).toString(), QStringLiteral("digiKam-5.9.0"));
}

void Exiv2ExtractorTest::testGPS()
{
    Exiv2Extractor plugin{this};

    SimpleExtractionResult result(testFilePath("test.jpg"), "image/jpeg");
    plugin.extract(&result);

    QCOMPARE(result.properties().value(PhotoGpsLatitude).toDouble(), 41.411);
    QCOMPARE(result.properties().value(PhotoGpsLongitude).toDouble(), 2.173);
    QCOMPARE(result.properties().value(PhotoGpsAltitude).toDouble(), 12.2);

    SimpleExtractionResult resultEmpty(testFilePath("test_no_gps.jpg"), "image/jpeg");
    plugin.extract(&resultEmpty);
    QVERIFY(!resultEmpty.properties().contains(PhotoGpsLatitude));
    QVERIFY(!resultEmpty.properties().contains(PhotoGpsLongitude));
    QVERIFY(!resultEmpty.properties().contains(PhotoGpsAltitude));

    SimpleExtractionResult resultZero(testFilePath("test_zero_gps.jpg"), "image/jpeg");
    plugin.extract(&resultZero);
    QVERIFY(resultZero.properties().contains(PhotoGpsLatitude));
    QVERIFY(resultZero.properties().contains(PhotoGpsLongitude));
    QVERIFY(resultZero.properties().contains(PhotoGpsAltitude));
    QCOMPARE(resultZero.properties().value(PhotoGpsLatitude).toDouble(), 0.0);
    QCOMPARE(resultZero.properties().value(PhotoGpsLongitude).toDouble(), 0.0);
    QCOMPARE(resultZero.properties().value(PhotoGpsAltitude).toDouble(), 0.0);
}

QTEST_GUILESS_MAIN(Exiv2ExtractorTest)
