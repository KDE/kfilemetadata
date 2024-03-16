/*
    This file is part of the Nepomuk KDE project.
    SPDX-FileCopyrightText: 2013 David Edmundson <davidedmundson@kde.org>
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include <QObject>
#include <QString>
#include <QTest>
#include <QTemporaryFile>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/plaintextextractor.h"

using namespace KFileMetaData;

class IndexerExtractorTests : public QObject
{
    Q_OBJECT
public:
    explicit IndexerExtractorTests(QObject* parent = nullptr);

private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void benchMarkPlainTextExtractor();
    void testPlainTextExtractor();
    void testPlainTextExtractor_data();
};

IndexerExtractorTests::IndexerExtractorTests(QObject* parent) :
    QObject(parent)
{
}

QString IndexerExtractorTests::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void IndexerExtractorTests::benchMarkPlainTextExtractor()
{
    PlainTextExtractor plugin(this);

    // generate a test file with varying number of words per line
    QTemporaryFile file(QStringLiteral("XXXXXX.txt"));
    QVERIFY(file.open());
    QByteArray chunk("foo bar ");
    for (int line = 0; line < 10000; ++line) {
        for (int i = 0; i < line % 100; ++i) {
            file.write(chunk);
        }
        file.write("\n");
    }

    SimpleExtractionResult result(file.fileName(), QStringLiteral("text/plain"));

    QBENCHMARK {
        plugin.extract(&result);
    }
}

void IndexerExtractorTests::testPlainTextExtractor()
{
    QFETCH(QString, fileName);
    QFETCH(ExtractionResult::Flags, flags);
    QFETCH(QString, content);
    QFETCH(QVariant, lineCount);

    PlainTextExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(fileName), QStringLiteral("text/plain"), flags);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Text);

    if (lineCount.isValid()) {
        QCOMPARE(result.properties().size(), 1);
        QCOMPARE(result.properties().value(Property::LineCount), lineCount);
    } else {
        QCOMPARE(result.properties().size(), 0);
    }

    QCOMPARE(result.text(), content);
}

void IndexerExtractorTests::testPlainTextExtractor_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<ExtractionResult::Flags>("flags");
    QTest::addColumn<QString>("content");
    QTest::addColumn<QVariant>("lineCount");

    using ER = ExtractionResult;

    const QString plainTextContent{"This is a text file "
                                   "it is four lines long "
                                   "it has 77 characters "
                                   "and 17 words. "};

    QTest::addRow("No extraction")     << QStringLiteral("plain_text_file.txt") << ER::Flags{ER::ExtractNothing}  << QString() << QVariant();
    QTest::addRow("Metadata only")     << QStringLiteral("plain_text_file.txt") << ER::Flags{ER::ExtractMetaData} << QString() << QVariant();
    QTest::addRow("Metadata and text") << QStringLiteral("plain_text_file.txt") << ER::Flags{ER::ExtractMetaData | ER::ExtractPlainText} << plainTextContent << QVariant(4);

    const QString emptyLineContent{"This is a text file  It is ten lines long, "
                                   "including three empty lines.  It has 152 "
                                   "characters and 28 words.  The file ends "
                                   "without a newline character. "};

    QTest::addRow("Count empty lines") << QStringLiteral("test_plain_text_newlines.txt") << ER::Flags{ER::ExtractMetaData | ER::ExtractPlainText} << emptyLineContent << QVariant(10);
}

QTEST_GUILESS_MAIN(IndexerExtractorTests)

#include "indexerextractortests.moc"
