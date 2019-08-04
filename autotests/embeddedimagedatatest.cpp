/*
 * EmbeddedImageData tests.
 *
 * Copyright (C) 2018  Alexander Stippich <a.stippich@gmx.net>
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

#include "embeddedimagedatatest.h"
#include "embeddedimagedata.h"
#include "indexerextractortestsconfig.h"

#include <QMimeDatabase>
#include <QTest>
#include <QByteArray>

using namespace KFileMetaData;

QString EmbeddedImageDataTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void EmbeddedImageDataTest::test()
{
    QFETCH(QString, fileName);
    QMimeDatabase mimeDb;
    QString testAudioFile;
    EmbeddedImageData imageData;
    QMap<EmbeddedImageData::ImageType, QByteArray> images;
    QByteArray originalFrontCoverImage;
    QFile testFile(testFilePath("cover.jpg"));
    testFile.open(QIODevice::ReadOnly);
    originalFrontCoverImage = testFile.readAll();

    testAudioFile = testFilePath(fileName);
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);
}

void EmbeddedImageDataTest::test_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("aiff")
            << QStringLiteral("test.aif")
            ;

    QTest::addRow("ape")
            << QStringLiteral("test.ape")
            ;

    QTest::addRow("opus")
            << QStringLiteral("test.opus")
            ;

    QTest::addRow("ogg")
            << QStringLiteral("test.ogg")
            ;

    QTest::addRow("flac")
            << QStringLiteral("test.flac")
            ;

    QTest::addRow("mp3")
            << QStringLiteral("test.mp3")
            ;

    QTest::addRow("m4a")
            << QStringLiteral("test.m4a")
            ;

    QTest::addRow("mpc")
            << QStringLiteral("test.mpc")
            ;

    QTest::addRow("speex")
            << QStringLiteral("test.spx")
            ;

    QTest::addRow("wav")
            << QStringLiteral("test.wav")
            ;

    QTest::addRow("wavpack")
            << QStringLiteral("test.wv")
            ;

    QTest::addRow("wma")
            << QStringLiteral("test.wma")
            ;
}

void EmbeddedImageDataTest::testWrite()
{
    QFETCH(QString, fileName);
    EmbeddedImageData imageData;

    QString testFileName = testFilePath(QStringLiteral("writer") + fileName);

    QFile::copy(testFilePath(fileName), testFileName);

    QFile testFile(testFilePath("test.jpg"));
    testFile.open(QIODevice::ReadOnly);

    QMap<EmbeddedImageData::ImageType, QByteArray> writeImages;
    QMap<EmbeddedImageData::ImageType, QByteArray> readImages;

    writeImages.insert(EmbeddedImageData::ImageType::FrontCover, testFile.readAll());
    imageData.writeImageData(testFileName, writeImages);
    readImages = imageData.imageData(testFileName);

    QCOMPARE(readImages.value(EmbeddedImageData::FrontCover), writeImages.value(EmbeddedImageData::FrontCover));

    QFile::remove(testFileName);
}

void EmbeddedImageDataTest::testWrite_data()
{
    QTest::addColumn<QString>("fileName");

    QTest::addRow("aiff")
            << QStringLiteral("test.aif")
            ;

    QTest::addRow("ape")
            << QStringLiteral("test.ape")
            ;

    QTest::addRow("opus")
            << QStringLiteral("test.opus")
            ;

    QTest::addRow("ogg")
            << QStringLiteral("test.ogg")
            ;

    QTest::addRow("flac")
            << QStringLiteral("test.flac")
            ;

    QTest::addRow("mp3")
            << QStringLiteral("test.mp3")
            ;

    QTest::addRow("m4a")
            << QStringLiteral("test.m4a")
            ;

    QTest::addRow("mpc")
            << QStringLiteral("test.mpc")
            ;

    QTest::addRow("speex")
            << QStringLiteral("test.spx")
            ;

    QTest::addRow("wav")
            << QStringLiteral("test.wav")
            ;

    QTest::addRow("wavpack")
            << QStringLiteral("test.wv")
            ;

    QTest::addRow("wma")
            << QStringLiteral("test.wma")
            ;
}

QTEST_GUILESS_MAIN(EmbeddedImageDataTest)
