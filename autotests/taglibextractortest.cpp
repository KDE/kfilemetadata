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
#include "indexerextractortestsconfig.h"
#include "extractors/taglibextractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>

using namespace KFileMetaData;

QString TagLibExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void TagLibExtractorTest::test()
{
    QScopedPointer<ExtractorPlugin> plugin(new TagLibExtractor(this));

    SimpleExtractionResult resultOpus(testFilePath("test.opus"), "audio/opus");
    plugin->extract(&resultOpus);

    QCOMPARE(resultOpus.types().size(), 1);
    QCOMPARE(resultOpus.types().first(), Type::Audio);

    QCOMPARE(resultOpus.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultOpus.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultOpus.properties().value(Property::Album), QVariant(QStringLiteral("Test")));
    QCOMPARE(resultOpus.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultOpus.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultOpus.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultOpus.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultOpus.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultOpus.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultOpus.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultOpus.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultFlac(testFilePath("test.flac"), "audio/flac");
    plugin->extract(&resultFlac);

    QCOMPARE(resultFlac.types().size(), 1);
    QCOMPARE(resultFlac.types().first(), Type::Audio);

    QCOMPARE(resultFlac.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultFlac.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultFlac.properties().value(Property::Album), QVariant(QStringLiteral("Test")));
    QCOMPARE(resultFlac.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultFlac.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultFlac.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultFlac.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultFlac.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultFlac.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultFlac.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultFlac.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultOgg(testFilePath("test.ogg"), "audio/ogg");
    plugin->extract(&resultOgg);

    QCOMPARE(resultOgg.types().size(), 1);
    QCOMPARE(resultOgg.types().first(), Type::Audio);

    QCOMPARE(resultOgg.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultOgg.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultOgg.properties().value(Property::Album), QVariant(QStringLiteral("Test")));
    QCOMPARE(resultOgg.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultOgg.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultOgg.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultOgg.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultOgg.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultOgg.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultOgg.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultOgg.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultMp3(testFilePath("test.mp3"), "audio/mpeg");
    plugin->extract(&resultMp3);

    QCOMPARE(resultMp3.types().size(), 1);
    QCOMPARE(resultMp3.types().first(), Type::Audio);

    QCOMPARE(resultMp3.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultMp3.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultMp3.properties().value(Property::Album), QVariant(QStringLiteral("Test")));
    QCOMPARE(resultMp3.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMp3.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultMp3.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultMp3.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMp3.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultMp3.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultMp3.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultMp3.properties().value(Property::DiscNumber).toInt(), 1);

    SimpleExtractionResult resultMpc(testFilePath("test.mpc"), "audio/x-musepack");
    plugin->extract(&resultMpc);

    QCOMPARE(resultMpc.types().size(), 1);
    QCOMPARE(resultMpc.types().first(), Type::Audio);

    QCOMPARE(resultMpc.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultMpc.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultMpc.properties().value(Property::Album), QVariant(QStringLiteral("Test")));
    QCOMPARE(resultMpc.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMpc.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultMpc.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultMpc.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMpc.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultMpc.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultMpc.properties().value(Property::Channels).toInt(), 1);
    QCOMPARE(resultMpc.properties().value(Property::DiscNumber).isValid(), false);

    SimpleExtractionResult resultMp4(testFilePath("test.m4a"), "audio/mp4");
    plugin->extract(&resultMp4);

    QCOMPARE(resultMp4.types().size(), 1);
    QCOMPARE(resultMp4.types().first(), Type::Audio);

    QCOMPARE(resultMp4.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(resultMp4.properties().value(Property::Artist), QVariant(QStringLiteral("Artist")));
    QCOMPARE(resultMp4.properties().value(Property::Album), QVariant(QStringLiteral("Test")));
    QCOMPARE(resultMp4.properties().value(Property::AlbumArtist), QVariant(QStringLiteral("Album Artist")));
    QCOMPARE(resultMp4.properties().value(Property::Genre), QVariant(QStringLiteral("Genre")));
    QCOMPARE(resultMp4.properties().value(Property::Comment), QVariant(QStringLiteral("Comment")));
    QCOMPARE(resultMp4.properties().value(Property::Composer), QVariant(QStringLiteral("Composer")));
    QCOMPARE(resultMp4.properties().value(Property::TrackNumber).toInt(), 1);
    QCOMPARE(resultMp4.properties().value(Property::ReleaseYear).toInt(), 2015);
    QCOMPARE(resultMp4.properties().value(Property::Channels).toInt(), 2);
    QCOMPARE(resultMp4.properties().value(Property::DiscNumber).toInt(), 1);
}

QTEST_GUILESS_MAIN(TagLibExtractorTest)
