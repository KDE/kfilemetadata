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
    const QString mimeType = mimeDb.mimeTypeForFile(testAudioFile).name();
    if (!imageData.mimeTypes().contains(mimeType)) {
        qWarning() << "mimeType" << mimeType << "not in imageData.mimeTypes()" << imageData.mimeTypes();
    }
    QVERIFY(imageData.mimeTypes().contains(mimeType));
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
    writeImages.insert(EmbeddedImageData::ImageType::FrontCover, imgFile.readAll());

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


QTEST_GUILESS_MAIN(EmbeddedImageDataTest)
