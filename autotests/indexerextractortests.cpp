/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2013 David Edmundson <davidedmundson@kde.org>
   Copyright (C) 2014 Vishesh Handa <me@vhanda.in>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "indexerextractortests.h"

#include <QtTest>

#include "simpleresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/plaintextextractor.h"

using namespace KFileMetaData;

IndexerExtractorTests::IndexerExtractorTests(QObject* parent) :
    QObject(parent)
{
}

QString IndexerExtractorTests::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void IndexerExtractorTests::benchMarkPlainTextExtractor()
{
    PlainTextExtractor plugin(this, QVariantList());

    // generate a test file with varying number of words per line
    QTemporaryFile file("XXXXXX.txt");
    QVERIFY(file.open());
    QByteArray chunk("foo bar ");
    for (int line = 0; line < 10000; ++line) {
        for (int i = 0; i < line % 100; ++i) {
            file.write(chunk);
        }
        file.write("\n");
    }

    SimpleResult result(file.fileName(), "text/plain");

    QBENCHMARK {
        plugin.extract(&result);
    }
}

void IndexerExtractorTests::testPlainTextExtractor()
{
    QScopedPointer<ExtractorPlugin> plugin(new PlainTextExtractor(this, QVariantList()));

    SimpleResult result(testFilePath("plain_text_file.txt"), "text/plain");
    plugin->extract(&result);

    QString content;
    QTextStream(&content) << "This is a text file\n"
                          << "it is four lines long\n"
                          << "it has 77 characters\n"
                          << "and 17 words.\n";

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Text);

    QCOMPARE(result.properties().size(), 1);
    QCOMPARE(result.properties().value(Property::LineCount), QVariant(4));

    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text(), content);
}

void IndexerExtractorTests::testPlainTextExtractorNoPlainText()
{
    QScopedPointer<ExtractorPlugin> plugin(new PlainTextExtractor(this, QVariantList()));

    SimpleResult result(testFilePath("plain_text_file.txt"), "text/plain", ExtractionResult::ExtractMetaData);
    plugin->extract(&result);

    QString content;
    QTextStream(&content) << "This is a text file\n"
                          << "it is four lines long\n"
                          << "it has 77 characters\n"
                          << "and 17 words.\n";

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Text);

    QCOMPARE(result.properties().size(), 0);
}

QTEST_MAIN(IndexerExtractorTests)

