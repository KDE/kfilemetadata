/*
 * TagLibExtractor tests.
 *
 * Copyright (C) 2015  Juan Palacios <jpalaciosdev@gmail.com>
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

#include "taglibextractortest.h"
#include "simpleextractionresult.h"
#include "propertyinfo.h"
//TODO: use QTESTFINDDATA and remove this
#include "indexerextractortestsconfig.h"
#include "extractors/taglibextractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>

Q_DECLARE_METATYPE(KFileMetaData::Property::Property)

using namespace KFileMetaData;

QString TagLibExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

const QStringList TagLibExtractorTest::propertyEnumNames(const QList<KFileMetaData::Property::Property>& keys) const
{
    QStringList result;
    for (auto key : keys) {
        result.append(PropertyInfo(key).name());
    }
    return result;
}

void TagLibExtractorTest::testPropertyTypes()
{
    TagLibExtractor plugin{this};

    SimpleExtractionResult resultOpus(testFilePath("test.opus"), "audio/opus");
    plugin.extract(&resultOpus);

    auto testForType = [](SimpleExtractionResult &result, Property::Property prop) {
            QCOMPARE(result.properties().value(prop).type(), PropertyInfo(prop).valueType());
    };

    QCOMPARE(resultOpus.types().size(), 1);
    QCOMPARE(resultOpus.types().constFirst(), Type::Audio);
    testForType(resultOpus, Property::Title);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Artist);
    testForType(resultOpus, Property::Album);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::AlbumArtist);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Genre);
    testForType(resultOpus, Property::Comment);
    testForType(resultOpus, Property::Composer);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Lyricist);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Conductor);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Arranger);
    testForType(resultOpus, Property::Ensemble);
    testForType(resultOpus, Property::Location);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Performer);
    testForType(resultOpus, Property::Langauge);
    testForType(resultOpus, Property::Publisher);
    testForType(resultOpus, Property::Label);
    QEXPECT_FAIL("", "Will be fixed in a following release", Continue);
    testForType(resultOpus, Property::Author);
    testForType(resultOpus, Property::Copyright);
    testForType(resultOpus, Property::Compilation);
    testForType(resultOpus, Property::License);
    testForType(resultOpus, Property::Opus);
    testForType(resultOpus, Property::TrackNumber);
    testForType(resultOpus, Property::ReleaseYear);
    testForType(resultOpus, Property::Channels);
    testForType(resultOpus, Property::DiscNumber);
    testForType(resultOpus, Property::Rating);
    testForType(resultOpus, Property::ReplayGainAlbumGain);
    testForType(resultOpus, Property::ReplayGainAlbumPeak);
    testForType(resultOpus, Property::ReplayGainTrackGain);
    testForType(resultOpus, Property::ReplayGainTrackPeak);
}

void TagLibExtractorTest::testCommonData()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString fileName = QStringLiteral("test.") + fileType;

    TagLibExtractor plugin{this};

    QCOMPARE(plugin.mimetypes().contains(mimeType), true);

    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Audio);

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(result.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(result.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(result.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(result.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(result.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(result.properties().value(Property::ReleaseYear).toInt(), 2015);
}

void TagLibExtractorTest::testCommonData_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("aiff")
        << QStringLiteral("aif")
        << QStringLiteral("audio/x-aiff")
        ;

    QTest::addRow("ape")
        << QStringLiteral("ape")
        << QStringLiteral("audio/x-ape")
        ;

    QTest::addRow("flac")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        ;

    QTest::addRow("m4a")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        ;

    QTest::addRow("mp3")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg3")
        ;

    QTest::addRow("mpc")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        ;

    QTest::addRow("ogg")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        ;

    QTest::addRow("opus")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        ;

    QTest::addRow("speex")
        << QStringLiteral("spx")
        << QStringLiteral("audio/speex")
        ;

    QTest::addRow("wav")
        << QStringLiteral("wav")
        << QStringLiteral("audio/wav")
        ;

    QTest::addRow("wavpack")
        << QStringLiteral("wv")
        << QStringLiteral("audio/x-wavpack")
        ;

    QTest::addRow("wma")
        << QStringLiteral("wma")
        << QStringLiteral("audio/x-ms-wma")
        ;
}

void TagLibExtractorTest::testVorbisComment()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString fileName = QStringLiteral("test.") + fileType;

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(result.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(result.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(result.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(result.properties().value(Property::Arranger), QVariant(QStringLiteral("Arranger")));
    QCOMPARE(result.properties().value(Property::Ensemble), QVariant(QStringLiteral("Ensemble")));
    QCOMPARE(result.properties().value(Property::Location), QVariant(QStringLiteral("Location")));
    QCOMPARE(result.properties().value(Property::Performer), QVariant(QStringLiteral("Performer")));
    QCOMPARE(result.properties().value(Property::Language), QVariant(QStringLiteral("Language")));
    QCOMPARE(result.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(result.properties().value(Property::Label), QVariant(QStringLiteral("Label")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(result.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(result.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(result.properties().value(Property::License), QVariant(QStringLiteral("License")));
    QCOMPARE(result.properties().value(Property::Lyrics), QVariant(QStringLiteral("Lyrics")));
    QCOMPARE(result.properties().value(Property::Opus).toInt(), 1);
    QCOMPARE(result.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(result.properties().value(Property::DiscNumber).toInt(), 1);
    QCOMPARE(result.properties().value(Property::Rating).toInt(), 5);
    QCOMPARE(result.properties().value(Property::ReplayGainAlbumGain), QVariant(-9.90));
    QCOMPARE(result.properties().value(Property::ReplayGainAlbumPeak), QVariant(1.512));
    QCOMPARE(result.properties().value(Property::ReplayGainTrackGain), QVariant(-10.44));
    QCOMPARE(result.properties().value(Property::ReplayGainTrackPeak), QVariant(1.301));
}

void TagLibExtractorTest::testVorbisComment_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("flac")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        ;

    QTest::addRow("ogg")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        ;

    QTest::addRow("opus")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        ;

    QTest::addRow("speex")
        << QStringLiteral("spx")
        << QStringLiteral("audio/speex")
        ;
}

void TagLibExtractorTest::testVorbisCommentMultivalue()
{
    QFETCH(QString, fileName);
    QFETCH(QString, mimeType);

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.properties().values(Property::Artist), QVariantList({QStringLiteral("Artist1"), QStringLiteral("Artist2")}));
    QCOMPARE(result.properties().values(Property::Genre), QVariantList({QStringLiteral("Genre1"), QStringLiteral("Genre2")}));
}

void TagLibExtractorTest::testVorbisCommentMultivalue_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("ogg multivalue")
        << QStringLiteral("test_multivalue.ogg")
        << QStringLiteral("audio/ogg")
        ;
}

void TagLibExtractorTest::testId3()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString fileName = QStringLiteral("test.") + fileType;

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(result.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(result.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(result.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(result.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(result.properties().value(Property::Language), QVariant(QStringLiteral("Language")));
    QCOMPARE(result.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(result.properties().value(Property::Lyrics), QVariant(QStringLiteral("Lyrics")));
    QCOMPARE(result.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(result.properties().value(Property::DiscNumber).toInt(), 1);
    QCOMPARE(result.properties().value(Property::Rating).toInt(), 10);
    QCOMPARE(result.properties().value(Property::ReplayGainAlbumGain), QVariant(-3.33));
    QCOMPARE(result.properties().value(Property::ReplayGainAlbumPeak), QVariant(1.333));
    QCOMPARE(result.properties().value(Property::ReplayGainTrackGain), QVariant(3.33));
    QCOMPARE(result.properties().value(Property::ReplayGainTrackPeak), QVariant(1.369));
}

void TagLibExtractorTest::testId3_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("aiff")
        << QStringLiteral("aif")
        << QStringLiteral("audio/x-aiff")
        ;

    QTest::addRow("mp3")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg")
        ;

    QTest::addRow("wav")
        << QStringLiteral("wav")
        << QStringLiteral("audio/wav")
        ;
}

void TagLibExtractorTest::testApe()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString fileName = QStringLiteral("test.") + fileType;

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(result.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(result.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(result.properties().value(Property::Arranger), QVariant(QStringLiteral("Arranger")));
    QCOMPARE(result.properties().value(Property::Ensemble), QVariant(QStringLiteral("Ensemble")));
    QCOMPARE(result.properties().value(Property::Location), QVariant(QStringLiteral("Location")));
    QCOMPARE(result.properties().value(Property::Performer), QVariant(QStringLiteral("Performer")));
    QCOMPARE(result.properties().value(Property::Language), QVariant(QStringLiteral("Language")));
    QCOMPARE(result.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(result.properties().value(Property::Label), QVariant(QStringLiteral("Label")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(result.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(result.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(result.properties().value(Property::License), QVariant(QStringLiteral("License")));
    QCOMPARE(result.properties().value(Property::Lyrics), QVariant(QStringLiteral("Lyrics")));
    QCOMPARE(result.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(result.properties().value(Property::DiscNumber).toInt(), 1);
    QCOMPARE(result.properties().value(Property::Rating).toInt(), 4);
    QCOMPARE(result.properties().value(Property::ReplayGainAlbumGain), QVariant(-9.44));
    QCOMPARE(result.properties().value(Property::ReplayGainAlbumPeak), QVariant(1.099));
    QCOMPARE(result.properties().value(Property::ReplayGainTrackGain), QVariant(-5.23));
    QCOMPARE(result.properties().value(Property::ReplayGainTrackPeak), QVariant(1.234));
}

void TagLibExtractorTest::testApe_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("ape")
        << QStringLiteral("ape")
        << QStringLiteral("audio/x-ape")
        ;

    QTest::addRow("musepack")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        ;

    QTest::addRow("wavpack")
        << QStringLiteral("wv")
        << QStringLiteral("audio/x-wavpack")
        ;
}

void TagLibExtractorTest::testMp4()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString fileName = QStringLiteral("test.") + fileType;

    TagLibExtractor plugin{this};
    SimpleExtractionResult resultMp4(testFilePath(fileName), mimeType);
    plugin.extract(&resultMp4);

    QCOMPARE(resultMp4.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMp4.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMp4.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(resultMp4.properties().value(Property::Lyrics), QVariant(QStringLiteral("Lyrics")));
    QCOMPARE(resultMp4.properties().value(Property::Channels).toInt(), 2);
    QCOMPARE(resultMp4.properties().value(Property::DiscNumber).toInt(), 1);
    QCOMPARE(resultMp4.properties().value(Property::Rating).toInt(), 8);
}

void TagLibExtractorTest::testMp4_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("mp4")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        ;
}

void TagLibExtractorTest::testAsf()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString fileName = QStringLiteral("test.") + fileType;

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(testFilePath(fileName), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(result.properties().value(Property::Rating).toInt(), 6);
    QCOMPARE(result.properties().value(Property::DiscNumber).toInt(), 1);
    QCOMPARE(result.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(result.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(result.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(result.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(result.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
}

void TagLibExtractorTest::testAsf_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("asf")
        << QStringLiteral("wma")
        << QStringLiteral("audio/x-ms-wma")
        ;
}

void TagLibExtractorTest::testId3Rating_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("expectedRating");

    QTest::addRow("WMP")
        << QFINDTESTDATA("samplefiles/mp3_rating/testWMP.mp3")
        << 0 ;
    QTest::addRow("WMP1")
        << QFINDTESTDATA("samplefiles/mp3_rating/testWMP1.mp3")
        << 2 ;
    QTest::addRow("WMP2")
        << QFINDTESTDATA("samplefiles/mp3_rating/testWMP2.mp3")
        << 4 ;
    QTest::addRow("WMP3")
        << QFINDTESTDATA("samplefiles/mp3_rating/testWMP3.mp3")
        << 6 ;
    QTest::addRow("WMP4")
        << QFINDTESTDATA("samplefiles/mp3_rating/testWMP4.mp3")
        << 8 ;
    QTest::addRow("WMP5")
        << QFINDTESTDATA("samplefiles/mp3_rating/testWMP5.mp3")
        << 10 ;
    QTest::addRow("MM")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM.mp3")
        << 0 ;
    QTest::addRow("MM1")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM1.mp3")
        << 1 ;
    QTest::addRow("MM2")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM2.mp3")
        << 2 ;
    QTest::addRow("MM3")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM3.mp3")
        << 3 ;
    QTest::addRow("MM4")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM4.mp3")
        << 4 ;
    QTest::addRow("MM5")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM5.mp3")
        << 5 ;
    QTest::addRow("MM6")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM6.mp3")
        << 6 ;
    QTest::addRow("MM7")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM7.mp3")
        << 7 ;
    QTest::addRow("MM8")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM8.mp3")
        << 8 ;
    QTest::addRow("MM9")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM9.mp3")
        << 9 ;
    QTest::addRow("MM10")
        << QFINDTESTDATA("samplefiles/mp3_rating/testMM10.mp3")
        << 10 ;
}

void TagLibExtractorTest::testId3Rating()
{
    QFETCH(QString, path);
    QFETCH(int, expectedRating);

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(path, "audio/mpeg");
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::Rating).toInt(), expectedRating);
}

void TagLibExtractorTest::testWmaRating()
{
    QFETCH(QString, path);
    QFETCH(int, expectedRating);

    TagLibExtractor plugin{this};
    SimpleExtractionResult result(path, "audio/x-ms-wma");
    plugin.extract(&result);

    QCOMPARE(result.properties().value(Property::Rating).toInt(), expectedRating);
}

void TagLibExtractorTest::testWmaRating_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<int>("expectedRating");

    QTest::addRow("WMP0")
        << QFINDTESTDATA("samplefiles/wma_rating/test0.wma")
        << 0 ;
    QTest::addRow("WMP1")
        << QFINDTESTDATA("samplefiles/wma_rating/test1.wma")
        << 2 ;
    QTest::addRow("WMP2")
        << QFINDTESTDATA("samplefiles/wma_rating/test2.wma")
        << 4 ;
    QTest::addRow("WMP3")
        << QFINDTESTDATA("samplefiles/wma_rating/test3.wma")
        << 6 ;
    QTest::addRow("WMP4")
        << QFINDTESTDATA("samplefiles/wma_rating/test4.wma")
        << 8 ;
    QTest::addRow("WMP5")
        << QFINDTESTDATA("samplefiles/wma_rating/test5.wma")
        << 10 ;
}

void TagLibExtractorTest::testNoMetadata_data()
{
    const auto expectedKeys = QList<Property::Property>{
            Property::BitRate,
            Property::Channels,
            Property::Duration,
            Property::SampleRate,
};

    QTest::addColumn<QString>("path");
    QTest::addColumn<QString>("mimeType");
    QTest::addColumn<QList<Property::Property>>("expectedKeys");
    QTest::addColumn<QString>("failMessage");

    QTest::addRow("mp3")
            << QFINDTESTDATA("samplefiles/no-meta/test.mp3")
            << QStringLiteral("audio/mpeg")
            << expectedKeys << QString()
               ;

    QTest::addRow("m4a")
            << QFINDTESTDATA("samplefiles/no-meta/test.m4a")
            << QStringLiteral("audio/mp4")
            << expectedKeys << QString()
               ;

    QTest::addRow("flac")
            << QFINDTESTDATA("samplefiles/no-meta/test.flac")
            << QStringLiteral("audio/flac")
            << expectedKeys << QString()
               ;

    QTest::addRow("opus")
            << QFINDTESTDATA("samplefiles/no-meta/test.opus")
            << QStringLiteral("audio/opus")
            << expectedKeys << QString()
               ;

    QTest::addRow("ogg")
            << QFINDTESTDATA("samplefiles/no-meta/test.ogg")
            << QStringLiteral("audio/ogg")
            << expectedKeys << QString()
               ;

    QTest::addRow("mpc")
            << QFINDTESTDATA("samplefiles/no-meta/test.mpc")
            << QStringLiteral("audio/x-musepack")
            << expectedKeys << QString()
               ;

}

void  TagLibExtractorTest::testNoMetadata()
{
    QFETCH(QString, path);
    QFETCH(QString, mimeType);
    QFETCH(QList<Property::Property>, expectedKeys);
    QFETCH(QString, failMessage);

    TagLibExtractor plugin{this};
    SimpleExtractionResult extracted(path, mimeType);
    plugin.extract(&extracted);

    const auto resultList = extracted.properties();
    const auto resultKeys = resultList.uniqueKeys();

    const auto excessKeys = resultKeys.toSet() - expectedKeys.toSet();
    const auto missingKeys = expectedKeys.toSet() - resultKeys.toSet();
    if (!excessKeys.isEmpty()) {
        const auto propNames =  propertyEnumNames(excessKeys.toList()).join(QLatin1String(", "));
        if (failMessage.isEmpty()) {
            const auto message = QStringLiteral("Excess properties: %1").arg(propNames);
            QWARN(qPrintable(message));
        } else {
            QEXPECT_FAIL("", qPrintable(QStringLiteral("%1: %2").arg(failMessage).arg(propNames)), Continue);
        }
    } else if (!missingKeys.isEmpty()) {
        const auto message = QStringLiteral("Missing properties: %1")
                .arg(propertyEnumNames(missingKeys.toList()).join(QLatin1String(", ")));
        QWARN(qPrintable(message));
    }
    QCOMPARE(resultKeys, expectedKeys);
    if (!failMessage.isEmpty()) {
        auto excessKeys = resultKeys.toSet() - expectedKeys.toSet();
        const auto message = QStringLiteral("%1: %2")
                .arg(failMessage)
                .arg(propertyEnumNames(excessKeys.toList()).join(QLatin1String(", ")));
        QEXPECT_FAIL("", qPrintable(message), Continue);
    }
    QCOMPARE(resultKeys, expectedKeys);
}

QTEST_GUILESS_MAIN(TagLibExtractorTest)
