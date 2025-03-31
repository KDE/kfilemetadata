/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/office2007extractor.h"
#include "mimeutils.h"

#include <QMimeDatabase>
#include <QObject>
#include <QTest>
#include <QTimeZone>

using namespace KFileMetaData;

class Office2007ExtractorTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testNoExtraction();
    void testNoExtraction_pptx();
    void testNoExtraction_xlsx();
    void test_docx_lo();
    void test_docx_word();
    void test_docx_word_strict();
    void test_pptx();
    void test_xlsx();
    void testMetaDataOnly();
    void test_xps();
    void test_oxps();
    void test_3mf();
};

QString Office2007ExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void Office2007ExtractorTest::testNoExtraction()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_libreoffice.docx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType, ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);
    QCOMPARE(result.properties().size(), 0);
}

void Office2007ExtractorTest::testNoExtraction_pptx()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_libreoffice.pptx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType, ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 2);
    QVERIFY(result.types().contains(Type::Document));
    QVERIFY(result.types().contains(Type::Presentation));
    QCOMPARE(result.properties().size(), 0);
}

void Office2007ExtractorTest::testNoExtraction_xlsx()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_libreoffice.xlsx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType, ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 2);
    QVERIFY(result.types().contains(Type::Document));
    QVERIFY(result.types().contains(Type::Spreadsheet));
    QCOMPARE(result.properties().size(), 0);
}

void Office2007ExtractorTest::test_docx_lo()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_libreoffice.docx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("KFileMetaData comment")));
    QCOMPARE(result.properties().value(Property::Language), QVariant(QStringLiteral("en-US")));
    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));
    QCOMPARE(result.properties().value(Property::PageCount), QVariant(1));
    QCOMPARE(result.properties().value(Property::WordCount), QVariant(4));
    QCOMPARE(result.properties().value(Property::LineCount), QVariant(1));

    QDateTime dt(QDate(2014, 07, 01), QTime(17, 37, 40), QTimeZone::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 10);
    QCOMPARE(result.text(), QStringLiteral("Test file for KFileMetaData. "));
}

void Office2007ExtractorTest::test_docx_word()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_word.docx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);

    QCOMPARE(result.properties().value(Property::Generator), QStringLiteral("Microsoft Word for the web"));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Creator Name")));
    QCOMPARE(result.properties().value(Property::CreationDate), QDateTime::fromString(QStringLiteral("2024-04-15T18:12:00+00"), Qt::ISODate));

    QCOMPARE(result.properties().size(), 3);
    QCOMPARE(result.text(), QStringLiteral("Testing This is a test document. First bullet point Second bullet point "));
}

void Office2007ExtractorTest::test_docx_word_strict()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_word_strict.docx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);

    QCOMPARE(result.properties().value(Property::Generator), QStringLiteral("Microsoft Office Word"));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("KFileMetaData Author")));
    QCOMPARE(result.properties().value(Property::CreationDate), QDateTime::fromString(QStringLiteral("2024-02-19T23:18:00+00"), Qt::ISODate));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Title), QStringLiteral("KFileMetaData Title"));
    QCOMPARE(result.properties().value(Property::LineCount), 4);
    QCOMPARE(result.properties().value(Property::WordCount), 14);
    QCOMPARE(result.properties().value(Property::PageCount), 1);
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("KFileMetaData comment")));

    QCOMPARE(result.properties().size(), 10);
    QCOMPARE(result.text(), QStringLiteral("Test file for KFileMetaData. Second paragraph. First bullet point Second bullet point "));
}

void Office2007ExtractorTest::test_pptx()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_libreoffice.pptx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 2);
    QVERIFY(result.types().contains(Type::Document));
    QVERIFY(result.types().contains(Type::Presentation));

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("KFileMetaData comment")));
    QCOMPARE(result.properties().value(Property::Language), QVariant(QStringLiteral("de-DE")));
    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));

    QDateTime dt(QDate(2014, 07, 02), QTime(10, 59, 23), QTimeZone::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 7);
    QCOMPARE(result.text(), QStringLiteral("KFileMetaData Pres Slide 2 Title Left list 1 1a 1b Right list 1 Right list 2 "));
}

void Office2007ExtractorTest::test_xlsx()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_libreoffice.xlsx"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 2);
    QVERIFY(result.types().contains(Type::Document));
    QVERIFY(result.types().contains(Type::Spreadsheet));

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("KFileMetaData comment")));
    QCOMPARE(result.properties().value(Property::Language), QVariant(QStringLiteral("de-DE")));
    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));

    QDateTime dt(QDate(2023, 11, 12), QTime(05, 59, 53), QTimeZone::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 7);
    QCOMPARE(result.text(), QStringLiteral("First cell Second cell Second row "));
}

void Office2007ExtractorTest::testMetaDataOnly()
{
    Office2007Extractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_libreoffice.docx")),
                        QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
                        ExtractionResult::ExtractMetaData);

    plugin.extract(&result);

    QVERIFY(!result.types().isEmpty());
    QVERIFY(!result.properties().isEmpty());
    QVERIFY(result.text().isEmpty());
}

void Office2007ExtractorTest::test_xps()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_xpsdocument.xps"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QVERIFY(result.types().contains(Type::Document));

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("Subject")));
}

void Office2007ExtractorTest::test_oxps()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_xpsdocument.oxps"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QVERIFY(result.types().contains(Type::Document));

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("Title")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("Subject")));
}

void Office2007ExtractorTest::test_3mf()
{
    Office2007Extractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_pyramid.3mf"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QVERIFY(result.types().contains(Type::Document));

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("3mf Pyramid")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Author")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("Subject")));
    const auto keywords = result.properties().values(Property::Keywords);
    QCOMPARE(keywords.size(), 2);
    QVERIFY(keywords.contains(QVariant(QStringLiteral("Keyword 1"))));
    QVERIFY(keywords.contains(QVariant(QStringLiteral("Keyword 2"))));
}

QTEST_GUILESS_MAIN(Office2007ExtractorTest)

#include "office2007extractortest.moc"
