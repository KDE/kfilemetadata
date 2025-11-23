/*
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-and-later
*/

#include <QTest>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"

#include "extractors/csvextractor.h"

using namespace KFileMetaData;
using namespace Qt::Literals::StringLiterals;

class CsvExtractorTests : public QObject
{
    Q_OBJECT
public:
    explicit CsvExtractorTests(QObject* parent = nullptr) : QObject(parent) {};

private Q_SLOTS:
    // void benchMarkCsvExtractor() {};
    void testCsvExtractor();
    void testCsvExtractor_data();
};

namespace {
QString testFilePath(const QString& fileName)
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}
}

void CsvExtractorTests::testCsvExtractor()
{
    QFETCH(QString, fileName);
    QFETCH(ExtractionResult::Flags, flags);
    QFETCH(QString, content);
    QFETCH(QVariant, lineCount);

    auto pathName = testFilePath(fileName);
    QVERIFY(QFile::exists(pathName));

    CsvExtractor plugin{this};

    SimpleExtractionResult result(pathName, QStringLiteral("text/csv"), flags);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 2);
    QCOMPARE(result.types(), (QList{Type::Document, Type::Spreadsheet}));

    if (lineCount.isValid()) {
        QCOMPARE(result.properties().size(), 1);
        QCOMPARE(result.properties().value(Property::LineCount), lineCount);
    } else {
        QCOMPARE(result.properties().size(), 0);
    }

    QCOMPARE(result.text(), content);
}

void CsvExtractorTests::testCsvExtractor_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<ExtractionResult::Flags>("flags");
    QTest::addColumn<QString>("content");
    QTest::addColumn<QVariant>("lineCount");

    using ER = ExtractionResult;

    auto content = u"A B C \n 1 2 3 \n third row second column third row, third column \n \"Double Quoted\" Unicode Text \u00C4\u00D6\u00DC\u00E4\u00F6\u00FC\u00DF  \n "_s;

    QTest::addRow("No extraction")               << u"test_csv_export_comma.csv"_s     << ER::Flags{ER::ExtractNothing}    << QString() << QVariant();
    QTest::addRow("Extract text with comma")     << u"test_csv_export_comma.csv"_s     << ER::Flags{ER::ExtractPlainText}  << content << QVariant();
    QTest::addRow("Extract text with semicolon") << u"test_csv_export_semicolon.csv"_s << ER::Flags{ER::ExtractPlainText}  << content << QVariant();
    QTest::addRow("Extract text with tabulator") << u"test_csv_export_tabulator.csv"_s << ER::Flags{ER::ExtractPlainText}  << content << QVariant();

    QTest::addRow("Extract text + meta with comma")     << u"test_csv_export_comma.csv"_s     << ER::Flags{ER::ExtractPlainText|ER::ExtractMetaData}  << content << QVariant(4);
    QTest::addRow("Extract text + meta with semicolon") << u"test_csv_export_semicolon.csv"_s << ER::Flags{ER::ExtractPlainText|ER::ExtractMetaData}  << content << QVariant(4);
    QTest::addRow("Extract text + meta with tabulator") << u"test_csv_export_tabulator.csv"_s << ER::Flags{ER::ExtractPlainText|ER::ExtractMetaData}  << content << QVariant(4);
}

QTEST_GUILESS_MAIN(CsvExtractorTests)

#include "csvextractortest.moc"
