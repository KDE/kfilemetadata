/*
 * Copyright (C) 2016 Varun Joshi <varunj.1011@gmail.com>
 * Copyright (C) 2018 Alexander Stippich <a.stippich@gmx.net>
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

#include "taglibwritertest.h"
#include "indexerextractortestsconfig.h"
#include "writers/taglibwriter.h"
#include "writedata.h"
#include <kfilemetadata/ExtractorCollection>
#include <kfilemetadata/Extractor>
#include <kfilemetadata/SimpleExtractionResult>
#include "taglib.h"
#include "fileref.h"

#include <QDebug>
#include <QTest>
#include <QDir>
#include <QFile>

using namespace KFileMetaData;

QString TagLibWriterTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void TagLibWriterTest::testCommonData()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);
    QFETCH(QString, stringSuffix);

    QString temporaryFileName = QStringLiteral("writertest.") + fileType;

    QFile::copy(testFilePath("test." + fileType), testFilePath(temporaryFileName));
    TagLibWriter writerPlugin{this};
    QCOMPARE(writerPlugin.writeMimetypes().contains(mimeType),true);

    WriteData data(testFilePath(temporaryFileName), mimeType);

    data.add(Property::Title, QString(QStringLiteral("Title1") + stringSuffix));
    data.add(Property::Artist, QString(QStringLiteral("Artist1") + stringSuffix));
    data.add(Property::Album, QString(QStringLiteral("Album1") + stringSuffix));
    data.add(Property::TrackNumber, 10);
    data.add(Property::ReleaseYear, 1999);
    data.add(Property::Genre, QString(QStringLiteral("Genre1") + stringSuffix));
    data.add(Property::Comment, QString(QStringLiteral("Comment1") + stringSuffix));
    writerPlugin.write(data);

    KFileMetaData::ExtractorCollection extractors;
    QList<KFileMetaData::Extractor*> extractorList = extractors.fetchExtractors(mimeType);
    if (extractorList.isEmpty())
        QFAIL("This mime type is not supported by the extractor. Likely a newer KDE Frameworks version is required.");
    KFileMetaData::Extractor* ex = extractorList.first();
    KFileMetaData::SimpleExtractionResult result(testFilePath(temporaryFileName), mimeType,
                                                 KFileMetaData::ExtractionResult::ExtractMetaData);

    ex->extract(&result);
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("Title1") + stringSuffix));
    QCOMPARE(result.properties().value(Property::Artist), QVariant(QStringLiteral("Artist1") + stringSuffix));
    QCOMPARE(result.properties().value(Property::Album), QVariant(QStringLiteral("Album1") + stringSuffix));
    QCOMPARE(result.properties().value(Property::TrackNumber).toInt(), 10);
    QCOMPARE(result.properties().value(Property::ReleaseYear).toInt(), 1999);
    QCOMPARE(result.properties().value(Property::Genre), QVariant(QStringLiteral("Genre1") + stringSuffix));
    QCOMPARE(result.properties().value(Property::Comment), QVariant(QStringLiteral("Comment1") + stringSuffix));

    QFile::remove(testFilePath(temporaryFileName));
}

void TagLibWriterTest::testCommonData_data()
{
    // Add some unicode characters, use codepoints to avoid any issues with
    // source encoding: "€µ"
    static const QChar data[2] = { 0x20ac, 0xb5 };
    QString unicodeTestStringSuffix(data, 2);

    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");
    QTest::addColumn<QString>("stringSuffix");


    QTest::addRow("aiff")
        << QStringLiteral("aif")
        << QStringLiteral("audio/x-aiff")
        << QString()
        ;

    QTest::addRow("aiff_unicode")
        << QStringLiteral("aif")
        << QStringLiteral("audio/x-aiff")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("ape")
        << QStringLiteral("ape")
        << QStringLiteral("audio/x-ape")
        << QString()
        ;

    QTest::addRow("ape_unicode")
        << QStringLiteral("ape")
        << QStringLiteral("audio/x-ape")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("flac")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        << QString()
        ;

    QTest::addRow("flac_unicode")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("m4a")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        << QString()
        ;

    QTest::addRow("m4a_unicode")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("mp3")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg3")
        << QString()
        ;

    QTest::addRow("mp3_unicode")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg3")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("mpc")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        << QString()
        ;

    QTest::addRow("mpc_unicode")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("ogg")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        << QString()
        ;

    QTest::addRow("ogg_unicode")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("opus")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        << QString()
        ;

    QTest::addRow("opus_unicode")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("speex")
        << QStringLiteral("spx")
        << QStringLiteral("audio/speex")
        << QString()
        ;

    QTest::addRow("speex_unicode")
        << QStringLiteral("spx")
        << QStringLiteral("audio/speex")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("wav")
        << QStringLiteral("wav")
        << QStringLiteral("audio/wav")
        << QString()
        ;

    QTest::addRow("wav_unicode")
        << QStringLiteral("wav")
        << QStringLiteral("audio/wav")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("wavpack")
        << QStringLiteral("wv")
        << QStringLiteral("audio/x-wavpack")
        << QString()
        ;

    QTest::addRow("wavpack_unicode")
        << QStringLiteral("wv")
        << QStringLiteral("audio/x-wavpack")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("wma")
        << QStringLiteral("wma")
        << QStringLiteral("audio/x-ms-wma")
        << QString()
        ;

    QTest::addRow("wma_unicode")
        << QStringLiteral("wma")
        << QStringLiteral("audio/x-ms-wma")
        << unicodeTestStringSuffix
        ;
}

QTEST_GUILESS_MAIN(TagLibWriterTest)
