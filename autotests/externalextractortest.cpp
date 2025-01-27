/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "externalextractor.h"

#include <QTest>

class ExternalExtractorTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test();
};

using namespace KFileMetaData;

namespace {
QString testFilePath(const QString& fileName)
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

QString pluginPath(const QString& fileName)
{
    return QLatin1String(INDEXER_TESTS_EXTERNALPLUGIN_PATH) + QLatin1Char('/') + fileName;
}
}

void ExternalExtractorTest::test()
{
    ExternalExtractor plugin{pluginPath(QStringLiteral("testexternalextractor"))};
    QVERIFY(plugin.mimetypes().contains(QStringLiteral("application/text")));

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_plain_text_file.txt")), QStringLiteral("application/text"));
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Text);

    QCOMPARE(result.text(), QStringLiteral("This is a text file\nit is four lines long\nit has 77 characters\nand 17 words.\n "));

    QCOMPARE(result.properties().value(Property::LineCount), 4);
}

QTEST_GUILESS_MAIN(ExternalExtractorTest)

#include "externalextractortest.moc"
