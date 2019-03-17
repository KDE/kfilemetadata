/*
 * Copyright (C) 2019 Alexander Stippich <a.stippich@gmx.net>
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

#include "ffmpegextractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/ffmpegextractor.h"

#include <QTest>

using namespace KFileMetaData;

namespace {

QString testFilePath(const QString& baseName, const QString& extension)
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH)
        + QLatin1Char('/') + baseName + QLatin1Char('.') + extension;
}

} // namespace

void ffmpegExtractorTest::videoTest_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("WebM")
        << QStringLiteral("webm")
        << QStringLiteral("video/webm");

    QTest::addRow("Matroska Video")
        << QStringLiteral("mkv")
        << QStringLiteral("video/x-matroska");
}

void ffmpegExtractorTest::videoTest()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    FFmpegExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QLatin1String("test"), fileType), mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Video);

    QCOMPARE(result.properties().value(Property::Title).toString(), QStringLiteral("Title"));
    QCOMPARE(result.properties().value(Property::Copyright).toString(), QStringLiteral("Copyright"));
    QCOMPARE(result.properties().value(Property::Author).toString(), QStringLiteral("Author"));
    QCOMPARE(result.properties().value(Property::ReleaseYear).toInt(), 2019);

    QCOMPARE(result.properties().value(Property::Width).toInt(), 1280);
    QCOMPARE(result.properties().value(Property::Height).toInt(), 720);
    QCOMPARE(result.properties().value(Property::FrameRate).toDouble(), 24.0/1.001);
    QCOMPARE(result.properties().value(Property::AspectRatio).toDouble(), 16.0/9);
}

QTEST_GUILESS_MAIN(ffmpegExtractorTest)
