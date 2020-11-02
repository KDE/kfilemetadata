/*
    EmbeddedImageData tests.

    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
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

void EmbeddedImageDataTest::initTestCase()
{
    QFile imgFile(testFilePath("cover.jpg"));
    imgFile.open(QIODevice::ReadOnly);
    m_coverImage = imgFile.readAll();
}

void EmbeddedImageDataTest::test()
{
    QFETCH(QString, fileName);
    QMimeDatabase mimeDb;
    QString testAudioFile;
    EmbeddedImageData imageData;
    QMap<EmbeddedImageData::ImageType, QByteArray> images;

    testAudioFile = testFilePath(fileName);
    const QString mimeType = mimeDb.mimeTypeForFile(testAudioFile).name();
    if (!imageData.mimeTypes().contains(mimeType)) {
        qWarning() << "mimeType" << mimeType << "not in imageData.mimeTypes()" << imageData.mimeTypes();
    }
    QVERIFY(imageData.mimeTypes().contains(mimeType));
    images = imageData.imageData(testAudioFile);
    QCOMPARE(images.value(EmbeddedImageData::FrontCover), m_coverImage);
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

    QMap<EmbeddedImageData::ImageType, QByteArray> writeImages;
    QMap<EmbeddedImageData::ImageType, QByteArray> readImages;

    writeImages.insert(EmbeddedImageData::ImageType::FrontCover, m_coverImage);
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
void EmbeddedImageDataTest::testDelete()
{
    QFETCH(QString, fileName);
    EmbeddedImageData imageData;

    QString testFileName = testFilePath(QStringLiteral("writer_del_") + fileName);

    QFile::copy(testFilePath(fileName), testFileName);

    QMap<EmbeddedImageData::ImageType, QByteArray> writeImages;
    QMap<EmbeddedImageData::ImageType, QByteArray> readImages;

    writeImages.insert(EmbeddedImageData::ImageType::FrontCover, QByteArray());
    imageData.writeImageData(testFileName, writeImages);
    readImages = imageData.imageData(testFileName);

    QVERIFY(!readImages.contains(EmbeddedImageData::FrontCover));

    QFile::remove(testFileName);
}

void EmbeddedImageDataTest::testDelete_data()
{
    testWrite_data();
}

void EmbeddedImageDataTest::testDeleteInsert()
{
    QFETCH(QString, fileName);
    EmbeddedImageData imageData;

    QString testFileName = testFilePath(QStringLiteral("writer_delinsert_") + fileName);

    QFile::copy(testFilePath(fileName), testFileName);

    QFile imgFile(testFilePath("test.jpg"));
    imgFile.open(QIODevice::ReadOnly);

    QMap<EmbeddedImageData::ImageType, QByteArray> delImages;
    QMap<EmbeddedImageData::ImageType, QByteArray> writeImages;
    QMap<EmbeddedImageData::ImageType, QByteArray> readImages;

    delImages.insert(EmbeddedImageData::ImageType::FrontCover, QByteArray());
    writeImages.insert(EmbeddedImageData::ImageType::FrontCover, m_coverImage);

    imageData.writeImageData(testFileName, delImages);
    readImages = imageData.imageData(testFileName);

    QVERIFY(!readImages.contains(EmbeddedImageData::FrontCover));

    imageData.writeImageData(testFileName, writeImages);
    readImages = imageData.imageData(testFileName);

    QCOMPARE(readImages.value(EmbeddedImageData::FrontCover), writeImages.value(EmbeddedImageData::FrontCover));

    QFile::remove(testFileName);
}

void EmbeddedImageDataTest::testDeleteInsert_data()
{
    testWrite_data();
}

void EmbeddedImageDataTest::testMultiImage()
{
    QFETCH(QString, fileName);
    QFETCH(EmbeddedImageData::ImageTypes, imageTypes);
    EmbeddedImageData imageData;

    QString testFileName = testFilePath(QStringLiteral("writer_multiimage_") + fileName);

    QFile::copy(testFilePath(fileName), testFileName);

    QMap<EmbeddedImageData::ImageType, QByteArray> writeImages;

    auto readImages = imageData.imageData(testFileName);
    QVERIFY(readImages.contains(EmbeddedImageData::FrontCover));

    using Image = EmbeddedImageData::ImageType;
    for (auto type : { Image::Other, Image::BackCover }) {
	if (type & imageTypes) {
	    writeImages.insert(type, m_coverImage);
	}
    }
    imageData.writeImageData(testFileName, writeImages);

    readImages = imageData.imageData(testFileName);
    // Test if FrontCover still exists
    QVERIFY(readImages.contains(EmbeddedImageData::FrontCover));

    for (auto type : { Image::FrontCover, Image::Other, Image::BackCover }) {
	if (type & imageTypes) {
	    QVERIFY2(readImages.contains(type), qPrintable(QString("has imagetype %1").arg(type)));
	    QCOMPARE(readImages.value(type), m_coverImage);
	}
    }

    QFile::remove(testFileName);
}

void EmbeddedImageDataTest::testMultiImage_data()
{
    using ImageTypes = EmbeddedImageData::ImageTypes;
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<ImageTypes>("imageTypes");

    using Image = EmbeddedImageData::ImageType;
    QTest::addRow("aiff") << QStringLiteral("test.aif") << ImageTypes{ Image::Other, Image::Artist };
    QTest::addRow("opus") << QStringLiteral("test.opus") << ImageTypes{ Image::Other, Image::BackCover };
    QTest::addRow("flac") << QStringLiteral("test.flac") << ImageTypes{ Image::Other, Image::Composer };
    QTest::addRow("wma")  << QStringLiteral("test.wma") << ImageTypes{ Image::Other, Image::Band };
}


QTEST_GUILESS_MAIN(EmbeddedImageDataTest)
