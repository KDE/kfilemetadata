/*
    SPDX-FileCopyrightText: 2019 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "ffmpegextractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/ffmpegextractor.h"
#include "mimeutils.h"

#include <QTest>

using namespace KFileMetaData;

namespace {

QString testFilePath(const QString& baseName, const QString& extension)
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH)
        + QLatin1Char('/') + baseName + QLatin1Char('.') + extension;
}

} // namespace

void ffmpegExtractorTest::testNoExtraction()
{
    QString fileName = testFilePath(QStringLiteral("test"), QStringLiteral("webm"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();

    FFmpegExtractor plugin{this};
    QVERIFY(plugin.mimetypes().contains(plugin.getSupportedMimeType(mimeType)));

    SimpleExtractionResult result(fileName, mimeType, ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Video);
    QCOMPARE(result.properties().size(), 0);
}


void ffmpegExtractorTest::testVideoProperties_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("codec");
    QTest::addColumn<QString>("audioCodec");
    QTest::addColumn<QString>("colorSpace");

    QTest::addRow("WebM")
        << QStringLiteral("webm")
        << QStringLiteral("vp8")
        << QStringLiteral("vorbis")
        << QStringLiteral("bt709");

    QTest::addRow("Matroska Video")
        << QStringLiteral("mkv")
        << QStringLiteral("vp8")
        << QStringLiteral("vorbis")
        << QStringLiteral("bt709");

    QTest::addRow("Vorbis Video")
        << QStringLiteral("ogv")
        << QStringLiteral("theora")
        << QStringLiteral("vorbis")
        << QStringLiteral("unknown");

    QTest::addRow("MPEG Transport")
        << QStringLiteral("ts")
        << QStringLiteral("mpeg2video")
        << QStringLiteral("mp2")
        << QStringLiteral("unknown");

    QTest::addRow("MS AVI")
        << QStringLiteral("avi")
        << QStringLiteral("mpeg4")
        << QStringLiteral("mp3")
        << QStringLiteral("unknown");
}

// only for testing of intrinsic video properties
void ffmpegExtractorTest::testVideoProperties()
{
    QFETCH(QString, fileType);
    QFETCH(QString, codec);
    QFETCH(QString, audioCodec);
    QFETCH(QString, colorSpace);

    QString fileName = testFilePath(QStringLiteral("test"), fileType);
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();

    FFmpegExtractor plugin{this};

    QVERIFY(plugin.mimetypes().contains(plugin.getSupportedMimeType(mimeType)));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Video);

    QCOMPARE(result.properties().value(Property::Width).toInt(), 1280);
    QCOMPARE(result.properties().value(Property::Height).toInt(), 720);
    QCOMPARE(result.properties().value(Property::FrameRate).toDouble(), 24.0/1.001);
    QCOMPARE(result.properties().value(Property::AspectRatio).toDouble(), 16.0/9);
    QCOMPARE(result.properties().value(Property::VideoCodec), codec);
    QCOMPARE(result.properties().value(Property::AudioCodec), audioCodec);
    QCOMPARE(result.properties().value(Property::PixelFormat), QStringLiteral("yuv420p"));
    QCOMPARE(result.properties().value(Property::ColorSpace), colorSpace);
}

void ffmpegExtractorTest::testMetaData_data()
{
    QTest::addColumn<QString>("fileType");

    QTest::addRow("WebM")
        << QStringLiteral("webm");

    QTest::addRow("Matroska Video")
        << QStringLiteral("mkv");

    QTest::addRow("Vorbis Video")
        << QStringLiteral("ogv");
}

void ffmpegExtractorTest::testMetaData()
{
    QFETCH(QString, fileType);

    QString fileName = testFilePath(QStringLiteral("test"), fileType);
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();

    FFmpegExtractor plugin{this};

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::Title).toString(), QStringLiteral("Title"));
    QCOMPARE(result.properties().value(Property::Copyright).toString(), QStringLiteral("Copyright"));
    QCOMPARE(result.properties().value(Property::Author).toString(), QStringLiteral("Author"));
    QCOMPARE(result.properties().value(Property::Artist).toString(), QStringLiteral("Artist"));
    QCOMPARE(result.properties().value(Property::ReleaseYear).toInt(), 2019);
}
QTEST_GUILESS_MAIN(ffmpegExtractorTest)

#include "moc_ffmpegextractortest.cpp"
