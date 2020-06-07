/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>
    SPDX-FileCopyrightText: 2016 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "odfextractortest.h"

#include <QTest>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/odfextractor.h"
#include "mimeutils.h"

using namespace KFileMetaData;

QString OdfExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void OdfExtractorTest::testNoExtraction()
{
    OdfExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.odt"));
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType, ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);
    QCOMPARE(result.properties().size(),0);
}

void OdfExtractorTest::testText()
{
    OdfExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.odt"));
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("KFileMetaData description")));
    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));

    QDateTime dt(QDate(2014, 07, 01), QTime(17, 37, 40, 690));
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().value(Property::WordCount), QVariant(4));
    QCOMPARE(result.properties().value(Property::PageCount), QVariant(1));
    QCOMPARE(result.text(), QStringLiteral("Test file for KFileMetaData. "));
    QCOMPARE(result.properties().size(), 8);
}

void OdfExtractorTest::testTextMetaDataOnly()
{
    OdfExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.odt")), QStringLiteral("application/vnd.oasis.opendocument.text"), ExtractionResult::ExtractMetaData);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.properties().size(), 8);
    QVERIFY(result.text().isEmpty());
}

void OdfExtractorTest::testPresentation()
{
    OdfExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.odp"));
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 2);
    QCOMPARE(result.types().at(0), Type::Document);
    QCOMPARE(result.types().at(1), Type::Presentation);

    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));
    QDateTime dt(QDate(2014, 07, 02), QTime(10, 59, 23, 434));
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.text(), QStringLiteral("KFileMetaData Pres "));
}

void OdfExtractorTest::testTextMissingMetaNoCrash()
{
    OdfExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_missing_meta.odt")), QStringLiteral("application/vnd.oasis.opendocument.text"));
    plugin.extract(&result);
}

void OdfExtractorTest::testTextMissingContentNoCrash()
{
    OdfExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_missing_content.odt")), QStringLiteral("application/vnd.oasis.opendocument.text"));
    plugin.extract(&result);
}

QTEST_GUILESS_MAIN(OdfExtractorTest)
