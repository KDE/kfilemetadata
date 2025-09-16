/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/mobiextractor.h"
#include "mimeutils.h"

#include <QTest>
#include <QMimeDatabase>
#include <QMimeType>

using namespace KFileMetaData;

class MobiExtractorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test();
    void testText();
};

namespace {
QString testFilePath(const QString& fileName)
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}
} // namespace <anonymous>

void MobiExtractorTest::test()
{
    MobiExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.mobi"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Document);

    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Happy Man")));
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("The Big Brown Bear")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("Baloo KFileMetaData")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("Honey")));
    QCOMPARE(result.properties().value(Property::Copyright), QVariant(QStringLiteral("License")));

    QCOMPARE(result.properties().size(), 5);
}

void MobiExtractorTest::testText()
{
    MobiExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.mobi"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

#if !defined(ENABLE_TEXT_EXTRACTION)
    QSKIP("Text extraction disabled");
#endif

    QCOMPARE(result.text(), QStringLiteral("This is a sample PDF file for KFileMetaData.  "));
}

QTEST_GUILESS_MAIN(MobiExtractorTest)

#include "mobiextractortest.moc"
