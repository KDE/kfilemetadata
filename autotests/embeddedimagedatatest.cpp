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

using namespace KFileMetaData;

QString EmbeddedImageDataTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void EmbeddedImageDataTest::test()
{
    QMimeDatabase mimeDb;
    QString testAudioFile;
    EmbeddedImageData imageData;
    QMap<EmbeddedImageData::ImageType, QByteArray> images;
    QByteArray originalFrontCoverImage;
    QFile testFile(testFilePath("cover.jpg"));
    testFile.open(QIODevice::ReadOnly);
    originalFrontCoverImage = testFile.readAll();

    testAudioFile = testFilePath("test.opus");
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    testAudioFile = testFilePath("test.ogg");
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    testAudioFile = testFilePath("test.flac");
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    testAudioFile = testFilePath("test.mp3");
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    testAudioFile = testFilePath("test.m4a");
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    testAudioFile = testFilePath("test.mpc");
    QVERIFY(imageData.mimeTypes().contains(mimeDb.mimeTypeForFile(testAudioFile).name()));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);
}

QTEST_GUILESS_MAIN(EmbeddedImageDataTest)
