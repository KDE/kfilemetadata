/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/exiv2extractor.h"
#include "mimeutils.h"

#include <QTest>
#include <QMimeDatabase>
#include <QTimeZone>

using namespace KFileMetaData;
using namespace Qt::Literals::StringLiterals;

class Exiv2ExtractorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testNoExtraction();
    void test();
    void testGPS();
    void testJpegJxlProperties();
    void testJpegJxlProperties_data();
    void testHeifProperties();
    void testTimezone();
    void testTimezone_data();
};

namespace {
QString testFilePath(const QString& fileName)
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

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
    using namespace KFileMetaData::Property;

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
    QCOMPARE(result.properties().value(AssistiveAlternateDescription).toString(),
             QStringLiteral("A blurry mess with a black-gray-blue area at the top and beige at the bottom"));
}

void Exiv2ExtractorTest::testGPS()
{
    using namespace KFileMetaData::Property;

    Exiv2Extractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.jpg")), QStringLiteral("image/jpeg"));
    plugin.extract(&result);

    QCOMPARE(result.properties().value(PhotoGpsLatitude).toDouble(), 41.411);
    QCOMPARE(result.properties().value(PhotoGpsLongitude).toDouble(), 2.173);
    QCOMPARE(result.properties().value(PhotoGpsAltitude).toDouble(), 12.2);

    SimpleExtractionResult resultEmpty(testFilePath(QStringLiteral("test_no_gps.jpg")), QStringLiteral("image/jpeg"));
    plugin.extract(&resultEmpty);
    QVERIFY(!resultEmpty.properties().contains(PhotoGpsLatitude));
    QVERIFY(!resultEmpty.properties().contains(PhotoGpsLongitude));
    QVERIFY(!resultEmpty.properties().contains(PhotoGpsAltitude));

    SimpleExtractionResult resultZero(testFilePath(QStringLiteral("test_zero_gps.jpg")), QStringLiteral("image/jpeg"));
    plugin.extract(&resultZero);
    QVERIFY(resultZero.properties().contains(PhotoGpsLatitude));
    QVERIFY(resultZero.properties().contains(PhotoGpsLongitude));
    QVERIFY(resultZero.properties().contains(PhotoGpsAltitude));
    QCOMPARE(resultZero.properties().value(PhotoGpsLatitude).toDouble(), 0.0);
    QCOMPARE(resultZero.properties().value(PhotoGpsLongitude).toDouble(), 0.0);
    QCOMPARE(resultZero.properties().value(PhotoGpsAltitude).toDouble(), 0.0);
}

void Exiv2ExtractorTest::testJpegJxlProperties()
{
    QFETCH(QString, fileName);
    QFETCH(QString, mimeType);

    Exiv2Extractor plugin{this};

    if ((mimeType == QStringLiteral("image/jxl")) && !plugin.mimetypes().contains(QStringLiteral("image/jxl"))) {
        QSKIP("BMFF support required for JXL");
    }
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    const auto properties = result.properties();
    QCOMPARE(properties.size(), 30);

    auto verifyProperty = [&properties](KFileMetaData::Property::Property prop, const QVariant &value)
    {
	if (value.canConvert<float>()) {
	    QCOMPARE(properties.value(prop).toFloat(), value.toFloat());
	} else {
	    QCOMPARE(properties.value(prop), value);
	}
    };

    verifyProperty(Property::Artist, QStringLiteral("Artist"));
    verifyProperty(Property::Description, QStringLiteral("Description"));
    verifyProperty(Property::Copyright, QStringLiteral("Copyright"));
    verifyProperty(Property::Generator, QStringLiteral("digikam-5.9.0"));
    verifyProperty(Property::PhotoGpsLatitude, 41.411f);
    verifyProperty(Property::PhotoGpsLongitude, 2.173f);
    verifyProperty(Property::PhotoGpsAltitude, 12.2f);
    verifyProperty(Property::Width, 8);
    verifyProperty(Property::Height, 14);
    verifyProperty(Property::Manufacturer, QStringLiteral("LGE"));
    verifyProperty(Property::Model, QStringLiteral("Nexus 5"));
    verifyProperty(Property::ImageDateTime, QDateTime(QDate(2014, 10, 04), QTime(13, 47, 43, 0), QTimeZone::LocalTime));
    verifyProperty(Property::PhotoFlash, 0);
    verifyProperty(Property::PhotoPixelXDimension, 8);
    verifyProperty(Property::PhotoPixelYDimension, 14);
    verifyProperty(Property::PhotoDateTimeOriginal, QDateTime(QDate(2014, 10, 04), QTime(13, 47, 43, 0), QTimeZone::LocalTime));
    verifyProperty(Property::PhotoFocalLength, 4.f);
    verifyProperty(Property::PhotoFocalLengthIn35mmFilm, 10.f);
    verifyProperty(Property::PhotoExposureTime, 0.0027027f);
    verifyProperty(Property::PhotoFNumber, 1.f);
    verifyProperty(Property::PhotoApertureValue, 0.f);
    verifyProperty(Property::PhotoExposureBiasValue, 0.f);
    verifyProperty(Property::PhotoWhiteBalance, 0);
    verifyProperty(Property::PhotoMeteringMode, 1);
    verifyProperty(Property::PhotoISOSpeedRatings, 100);
    verifyProperty(Property::PhotoSaturation, 0);
    verifyProperty(Property::PhotoSharpness, 0);
    verifyProperty(Property::Title, QStringLiteral("Title"));
    verifyProperty(Property::Subject, QStringLiteral("Subject"));
    verifyProperty(Property::AssistiveAlternateDescription, QStringLiteral("A blurry mess with a black-gray-blue area at the top and beige at the bottom"));
}

void Exiv2ExtractorTest::testJpegJxlProperties_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("jpeg") << QStringLiteral("test.jpg") << QStringLiteral("image/jpeg");
    QTest::addRow("jxl") << QStringLiteral("test.jxl") << QStringLiteral("image/jxl");
    QTest::addRow("webp") << QStringLiteral("test.webp") << QStringLiteral("image/webp");
}

void Exiv2ExtractorTest::testHeifProperties()
{
    Exiv2Extractor plugin{this};

    if (!plugin.mimetypes().contains(QStringLiteral("image/heif"))) {
        QSKIP("BMFF support required for HEIF");
    }

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.heif")), QStringLiteral("image/heif"));
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    const auto properties = result.properties();
    QCOMPARE(properties.size(), 23);

    auto verifyProperty = [&properties](KFileMetaData::Property::Property prop, const QVariant &value)
    {
	if (value.canConvert<float>()) {
	    QCOMPARE(properties.value(prop).toFloat(), value.toFloat());
	} else {
	    QCOMPARE(properties.value(prop), value);
	}
    };

    verifyProperty(Property::Artist, QStringLiteral("Artist"));
    verifyProperty(Property::Description, QStringLiteral("Description"));
    verifyProperty(Property::Copyright, QStringLiteral("Copyright"));
    verifyProperty(Property::Generator, QStringLiteral("digikam-5.9.0"));
    verifyProperty(Property::PhotoGpsLatitude, 51.3331f);
    verifyProperty(Property::PhotoGpsLongitude, -0.705575f);
    verifyProperty(Property::PhotoGpsAltitude, 0.f);
    verifyProperty(Property::Width, 750);
    verifyProperty(Property::Height, 1000);
    verifyProperty(Property::Manufacturer, QStringLiteral("samsung"));
    verifyProperty(Property::Model, QStringLiteral("SM-J610N"));
    verifyProperty(Property::ImageDateTime, QDateTime(QDate(2022, 03, 24), QTime(18, 20, 07, 0), QTimeZone::LocalTime));
    verifyProperty(Property::PhotoFlash, 0);
    verifyProperty(Property::PhotoPixelXDimension, 750);
    verifyProperty(Property::PhotoPixelYDimension, 1000);
    verifyProperty(Property::PhotoDateTimeOriginal, QDateTime(QDate(2020, 03, 31), QTime(11, 14, 30, 0), QTimeZone::LocalTime));
    verifyProperty(Property::PhotoFocalLength, 3.6f);
    verifyProperty(Property::PhotoFocalLengthIn35mmFilm, 26.f);
    verifyProperty(Property::PhotoExposureTime, 0.00429185f);
    verifyProperty(Property::PhotoFNumber, 1.9f);
    verifyProperty(Property::PhotoApertureValue, 1.85f);
    verifyProperty(Property::PhotoExposureBiasValue, 0.f);
    verifyProperty(Property::PhotoWhiteBalance, 0);
    verifyProperty(Property::PhotoMeteringMode, 2);
    verifyProperty(Property::PhotoISOSpeedRatings, 40);
    verifyProperty(Property::PhotoSaturation, 0);
    verifyProperty(Property::PhotoSharpness, 0);
}

void Exiv2ExtractorTest::testTimezone()
{
    QFETCH(QString, fileName);
    QFETCH(QString, mimeType);
    QFETCH(QDateTime, dateTime);

    Exiv2Extractor plugin{this};
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    QCOMPARE(result.properties().value(Property::ImageDateTime), dateTime);
}

void Exiv2ExtractorTest::testTimezone_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("mimeType");
    QTest::addColumn<QDateTime>("dateTime");

    QTest::addRow("CanonTi") << u"test_canonti.jpg"_s << u"image/jpeg"_s << QDateTime(QDate(2025, 8, 2), QTime(15, 28, 31, 290), QTimeZone(7200));
}

QTEST_GUILESS_MAIN(Exiv2ExtractorTest)

#include "exiv2extractortest.moc"
