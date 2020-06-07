/*
    This file is part of the Nepomuk KDE project.
    SPDX-FileCopyrightText: 2013 David Edmundson <davidedmundson@kde.org>
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "indexerextractortests.h"

#include <QTest>
#include <QTemporaryFile>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/plaintextextractor.h"

using namespace KFileMetaData;

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

void IndexerExtractorTests::testNoExtraction()
{
    PlainTextExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("plain_text_file.txt")), QStringLiteral("text/plain"), ExtractionResult::ExtractNothing);
    plugin.extract(&result);


    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Text);

    QCOMPARE(result.properties().size(), 0);
}

void IndexerExtractorTests::testPlainTextExtractor()
{
    PlainTextExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("plain_text_file.txt")), QStringLiteral("text/plain"));
    plugin.extract(&result);

    QString content;
    QTextStream(&content) << "This is a text file\n"
                          << "it is four lines long\n"
                          << "it has 77 characters\n"
                          << "and 17 words.\n";

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Text);

    QCOMPARE(result.properties().size(), 1);
    QCOMPARE(result.properties().value(Property::LineCount), QVariant(4));

    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text(), content);
}

void IndexerExtractorTests::testPlainTextExtractorNoPlainText()
{
    PlainTextExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("plain_text_file.txt")), QStringLiteral("text/plain"), ExtractionResult::ExtractMetaData);
    plugin.extract(&result);

    QString content;
    QTextStream(&content) << "This is a text file\n"
                          << "it is four lines long\n"
                          << "it has 77 characters\n"
                          << "and 17 words.\n";

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Text);

    QCOMPARE(result.properties().size(), 0);
}

QTEST_GUILESS_MAIN(IndexerExtractorTests)

