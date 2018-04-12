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
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

const QStringList TagLibExtractorTest::propertyEnumNames(const QList<KFileMetaData::Property::Property>& keys) const
{
    QStringList result;
    for (auto key : keys) {
        result.append(PropertyInfo(key).name());
    }
    return result;
}

void TagLibExtractorTest::test()
{
    TagLibExtractor plugin{this};

    SimpleExtractionResult resultOpus(testFilePath("test.opus"), "audio/opus");
    plugin.extract(&resultOpus);

    QCOMPARE(resultOpus.types().size(), 1);
    QCOMPARE(resultOpus.types().constFirst(), Type::Audio);

    QCOMPARE(resultOpus.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultOpus.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultOpus.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(resultOpus.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultOpus.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultOpus.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultOpus.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultOpus.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(resultOpus.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(resultOpus.properties().value(Property::Arranger), QVariant(QStringLiteral("Arranger")));
    QCOMPARE(resultOpus.properties().value(Property::Ensemble), QVariant(QStringLiteral("Ensemble")));
    QCOMPARE(resultOpus.properties().value(Property::Location), QVariant(QStringLiteral("Location")));
    QCOMPARE(resultOpus.properties().value(Property::Performer), QVariant(QStringLiteral("Performer")));
    QCOMPARE(resultOpus.properties().value(Property::Langauge), QVariant(QStringLiteral("Language")));
    QCOMPARE(resultOpus.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(resultOpus.properties().value(Property::Label), QVariant(QStringLiteral("Label")));
    QCOMPARE(resultOpus.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(resultOpus.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(resultOpus.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(resultOpus.properties().value(Property::License), QVariant(QStringLiteral("License")));
    QCOMPARE(resultOpus.properties().value(Property::Opus).toInt(), 1);
    QCOMPARE(resultOpus.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultOpus.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultOpus.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultOpus.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultFlac(testFilePath("test.flac"), "audio/flac");
    plugin.extract(&resultFlac);

    QCOMPARE(resultFlac.types().size(), 1);
    QCOMPARE(resultFlac.types().constFirst(), Type::Audio);

    QCOMPARE(resultFlac.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultFlac.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultFlac.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(resultFlac.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultFlac.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultFlac.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultFlac.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultFlac.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(resultFlac.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(resultFlac.properties().value(Property::Arranger), QVariant(QStringLiteral("Arranger")));
    QCOMPARE(resultFlac.properties().value(Property::Ensemble), QVariant(QStringLiteral("Ensemble")));
    QCOMPARE(resultFlac.properties().value(Property::Location), QVariant(QStringLiteral("Location")));
    QCOMPARE(resultFlac.properties().value(Property::Performer), QVariant(QStringLiteral("Performer")));
    QCOMPARE(resultFlac.properties().value(Property::Langauge), QVariant(QStringLiteral("Language")));
    QCOMPARE(resultFlac.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(resultFlac.properties().value(Property::Label), QVariant(QStringLiteral("Label")));
    QCOMPARE(resultFlac.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(resultFlac.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(resultFlac.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(resultFlac.properties().value(Property::License), QVariant(QStringLiteral("License")));
    QCOMPARE(resultFlac.properties().value(Property::Opus).toInt(), 1);
    QCOMPARE(resultFlac.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultFlac.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultFlac.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultFlac.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultOgg(testFilePath("test.ogg"), "audio/ogg");
    plugin.extract(&resultOgg);

    QCOMPARE(resultOgg.types().size(), 1);
    QCOMPARE(resultOgg.types().constFirst(), Type::Audio);

    QCOMPARE(resultOgg.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultOgg.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultOgg.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(resultOgg.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultOgg.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultOgg.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultOgg.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultOgg.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(resultOgg.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(resultOgg.properties().value(Property::Arranger), QVariant(QStringLiteral("Arranger")));
    QCOMPARE(resultOgg.properties().value(Property::Ensemble), QVariant(QStringLiteral("Ensemble")));
    QCOMPARE(resultOgg.properties().value(Property::Location), QVariant(QStringLiteral("Location")));
    QCOMPARE(resultOgg.properties().value(Property::Performer), QVariant(QStringLiteral("Performer")));
    QCOMPARE(resultOgg.properties().value(Property::Langauge), QVariant(QStringLiteral("Language")));
    QCOMPARE(resultOgg.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(resultOgg.properties().value(Property::Label), QVariant(QStringLiteral("Label")));
    QCOMPARE(resultOgg.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(resultOgg.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(resultOgg.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(resultOgg.properties().value(Property::License), QVariant(QStringLiteral("License")));
    QCOMPARE(resultOgg.properties().value(Property::Opus).toInt(), 1);
    QCOMPARE(resultOgg.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultOgg.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultOgg.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultOgg.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultMp3(testFilePath("test.mp3"), "audio/mpeg");
    plugin.extract(&resultMp3);

    QCOMPARE(resultMp3.types().size(), 1);
    QCOMPARE(resultMp3.types().constFirst(), Type::Audio);

    QCOMPARE(resultMp3.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultMp3.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultMp3.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(resultMp3.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMp3.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultMp3.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultMp3.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMp3.properties().value(Property::Lyricist), QVariant(QStringLiteral("Lyricist")));
    QCOMPARE(resultMp3.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(resultMp3.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(resultMp3.properties().value(Property::Langauge), QVariant(QStringLiteral("Language")));
    QCOMPARE(resultMp3.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(resultMp3.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultMp3.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultMp3.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultMp3.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultMpc(testFilePath("test.mpc"), "audio/x-musepack");
    plugin.extract(&resultMpc);

    QCOMPARE(resultMpc.types().size(), 1);
    QCOMPARE(resultMpc.types().constFirst(), Type::Audio);

    QCOMPARE(resultMpc.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultMpc.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultMpc.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(resultMpc.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMpc.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultMpc.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultMpc.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMpc.properties().value(Property::Conductor), QVariant(QStringLiteral("Conductor")));
    QCOMPARE(resultMpc.properties().value(Property::Arranger), QVariant(QStringLiteral("Arranger")));
    QCOMPARE(resultMpc.properties().value(Property::Ensemble), QVariant(QStringLiteral("Ensemble")));
    QCOMPARE(resultMpc.properties().value(Property::Location), QVariant(QStringLiteral("Location")));
    QCOMPARE(resultMpc.properties().value(Property::Performer), QVariant(QStringLiteral("Performer")));
    QCOMPARE(resultMpc.properties().value(Property::Langauge), QVariant(QStringLiteral("Language")));
    QCOMPARE(resultMpc.properties().value(Property::Publisher), QVariant(QStringLiteral("Publisher")));
    QCOMPARE(resultMpc.properties().value(Property::Label), QVariant(QStringLiteral("Label")));
    QCOMPARE(resultMpc.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(resultMpc.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(resultMpc.properties().value(Property::Compilation), QVariant(QStringLiteral("Compilation")));
    QCOMPARE(resultMpc.properties().value(Property::License), QVariant(QStringLiteral("License")));
    QCOMPARE(resultMpc.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultMpc.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultMpc.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultMpc.properties().value(Property::DiscNumber).isValid(), false);

    SimpleExtractionResult resultMp4(testFilePath("test.m4a"), "audio/mp4");
    plugin.extract(&resultMp4);

    QCOMPARE(resultMp4.types().size(), 1);
    QCOMPARE(resultMp4.types().constFirst(), Type::Audio);

    QCOMPARE(resultMp4.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultMp4.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultMp4.properties().value(Property::Album), QVariant(QStringLiteral("Album")));
    QCOMPARE(resultMp4.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMp4.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultMp4.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultMp4.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMp4.properties().value(Property::Copyright), QVariant(QStringLiteral("Copyright")));
    QCOMPARE(resultMp4.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultMp4.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultMp4.properties().value(Property::Channels).toInt(), 2);
    QCOMPARE(resultMp4.properties().value(Property::DiscNumber).toInt(), 1);
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
        << QStringLiteral("audio/mp3")
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
    if (excessKeys.size()) {
        const auto propNames =  propertyEnumNames(excessKeys.toList()).join(QLatin1String(", "));
        if (failMessage.isEmpty()) {
            const auto message = QStringLiteral("Excess properties: %1").arg(propNames);
            QWARN(qPrintable(message));
        } else {
            QEXPECT_FAIL("", qPrintable(QStringLiteral("%1: %2").arg(failMessage).arg(propNames)), Continue);
        }
    } else if (missingKeys.size()) {
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
