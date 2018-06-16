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

#include <QTest>

using namespace KFileMetaData;

QString EmbeddedImageDataTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void EmbeddedImageDataTest::test()
{
    QMap<EmbeddedImageData::ImageType, QByteArray> images;
    QByteArray originalFrontCoverImage;
    QFile testFile(testFilePath("test.jpg"));
    testFile.open(QIODevice::ReadOnly);
    originalFrontCoverImage = testFile.readAll();

    EmbeddedImageData imageData;
    images = imageData.imageData(testFilePath("test.opus"));
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    images = imageData.imageData(testFilePath("test.ogg"));
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    images = imageData.imageData(testFilePath("test.flac"));
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    images = imageData.imageData(testFilePath("test.mp3"));
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    images = imageData.imageData(testFilePath("test.m4a"));
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);

    images = imageData.imageData(testFilePath("test.mpc"));
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), originalFrontCoverImage);
}

QTEST_GUILESS_MAIN(EmbeddedImageDataTest)
