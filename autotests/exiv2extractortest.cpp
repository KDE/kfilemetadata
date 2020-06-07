/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "exiv2extractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/exiv2extractor.h"
#include "mimeutils.h"

#include <QTest>
#include <QMimeDatabase>

using namespace KFileMetaData;
using namespace KFileMetaData::Property;

QString Exiv2ExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void Exiv2ExtractorTest::testNoExtraction()
{
    Exiv2Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.jpg"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType, ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);
    QCOMPARE(result.properties().size(), 0);
}

void Exiv2ExtractorTest::test()
{
    Exiv2Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.jpg"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
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
