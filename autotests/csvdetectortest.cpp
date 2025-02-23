/*
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-and-later
*/

#include <QTest>

#include "../src/extractors/csvdecoder_p.h"

using namespace KFileMetaData;
using namespace Qt::Literals::StringLiterals;
using CsvStyle = KFileMetaData::Helper::CsvStyle;

class CsvDetectorTests : public QObject
{
    Q_OBJECT
public:
    explicit CsvDetectorTests(QObject* parent = nullptr) : QObject(parent) {};

private Q_SLOTS:
    void detectFieldSeparator();
    void detectFieldSeparator_data();

    void detectLineSeparator();
    void detectLineSeparator_data();

    void decodeCsv();
    void decodeCsv_data();

    void decodeCsvChunked();
    void decodeCsvChunked_data();
};

void CsvDetectorTests::detectFieldSeparator()
{
    using KFileMetaData::Helper::CsvStyle;
    QFETCH(QString, input);
    QFETCH(bool, usesComma);

    auto res = Helper::detectCsvStyle(input);

    QCOMPARE(res.delimiter, usesComma ? CsvStyle::Comma : CsvStyle::Semicolon);
}

void CsvDetectorTests::detectFieldSeparator_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("usesComma");

    struct TestData {
        const char *description;
        QStringList fields;
    };

    TestData testData[] = {
        {"unquoted",            {u"1234"_s, u"a b"_s, u"x"_s}},
        {"empty fields",        {{}, {}, u"x"_s}},
        {"minimal quoted",      {u"1234"_s, u"\"a, b and c\""_s, u"x"_s}},
        {"all quoted",          {u"\"1234\""_s, u"\"a b\""_s, u"\"x\""_s}},

        {"comma in quote",      {u"\"1234\""_s, u"\"a, b and c\""_s, u"\"x\""_s}},
        {"semicolon in quote",  {u"\"1234\""_s, u"\"a; b and c\""_s, u"\"x\""_s}},

        {"quote in quote",      {u"1234"_s, u"\"\"\"\""_s, u"x"_s}},
        {"multiple quotes",     {u"1234"_s, u"\"a \"\"\"\" b\""_s, u"x"_s}},

        {"newline in quote",    {u"1234"_s, u"\"a \n b\""_s, u"x"_s}},
        {"crnl in quote",       {u"1234"_s, u"\"a \r\n b\""_s, u"x"_s}},
    };

    for (const auto &row : testData) {
        auto input = row.fields.join(u","_s);
        QTest::addRow("%s / ','", row.description) << input << true;
    }
    for (const auto &row : testData) {
        auto input = row.fields.join(u";"_s);
        QTest::addRow("%s / ';'", row.description) << input << false;
    }

    // The first row is ambigous on its own '1,2';'3,4';'5' or '1','2;3','4;5'
    QTest::addRow("ambiguous first row") << QStringLiteral("1,2;3,4;5\n") << true;
    QTest::addRow("ambiguous first row, comma") << QStringLiteral("1,2;3,4;5\na,b,c\n") << true;
    QTest::addRow("ambiguous first row, semicolon") << QStringLiteral("1,2;3,4;5\na;b;c\n") << false;
}

void CsvDetectorTests::detectLineSeparator()
{
    QFETCH(QString, input);
    QFETCH(CsvStyle::LineTerminator, crnl);

    auto res = Helper::detectCsvStyle(input);

    QCOMPARE(res.lineTerminator, crnl);
}

void CsvDetectorTests::detectLineSeparator_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<CsvStyle::LineTerminator>("crnl");

    struct TestData {
        const char *description;
        QStringList lines;
    };

    TestData testData[] = {
        {"unquoted", {u"1234,a b"_s, u"x,y"_s}},
        {"quoted",   {u"\"1234\",\"a b\""_s, u"\"x\",\"y\""_s}},

        {"linewraps in quote", {u"\"\r\n\",\"\r\""_s, u"\"n\","_s}},
        {"linewrap after quote", {u"\"1234,\",\"\""_s, u"\",x\","_s}},
    };

    for (const auto &row : testData) {
        auto input = row.lines.join(QStringLiteral("\r"));
        QTest::addRow("%s / 'cr", row.description) << input << CsvStyle::CR;
    }
    for (const auto &row : testData) {
        auto input = row.lines.join(QStringLiteral("\n"));
        QTest::addRow("%s / 'nl", row.description) << input << CsvStyle::NL;
    }
    for (const auto &row : testData) {
        auto input = row.lines.join(QStringLiteral("\r\n"));
        QTest::addRow("%s / 'crnl", row.description) << input << CsvStyle::CRNL;
    }
}

void CsvDetectorTests::decodeCsv()
{
    using KFileMetaData::Helper::CsvStyle;
    QFETCH(QString, input);
    QFETCH(QList<QStringList>, expected);

    auto [rows, end] = Helper::decodeCsv(input, {CsvStyle::Comma, CsvStyle::NL});

    QCOMPARE(rows.size(), expected.size());
    for (int i = 0; i < rows.size(); i++) {
        QCOMPARE(rows[i], expected[i]);
    }
}

void CsvDetectorTests::decodeCsv_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QList<QStringList>>("expected");

    QTest::addRow("RowCount 0 NL") << u"1234,5678"_s              << QList<QStringList>{{u"1234"_s, u"5678"_s}};
    QTest::addRow("RowCount 1 NL") << u"1234,5678\nabcd,efgh"_s   << QList<QStringList>{{u"1234"_s, u"5678"_s},{u"abcd"_s, u"efgh"_s}};
    QTest::addRow("RowCount 2 NL") << u"1234,5678\nabcd,efgh\n"_s << QList<QStringList>{{u"1234"_s, u"5678"_s},{u"abcd"_s, u"efgh"_s}};

    // Permute quoting
    QTest::addRow("CheckQuotes 1") << u"\"12,34\",5678\n\"ab,cd\",efgh\n"_s         << QList<QStringList>{{u"12,34"_s, u"5678"_s},{u"ab,cd"_s, u"efgh"_s}};
    QTest::addRow("CheckQuotes 2") << u"\"12,34\",\"5678\"\n\"ab,cd\",\"efgh\"\n"_s << QList<QStringList>{{u"12,34"_s, u"5678"_s},{u"ab,cd"_s, u"efgh"_s}};
    QTest::addRow("CheckQuotes 3") << u"1234,\"56,78\"\nabcd,\"ef,gh\"\n"_s         << QList<QStringList>{{u"1234"_s, u"56,78"_s},{u"abcd"_s, u"ef,gh"_s}};
    QTest::addRow("CheckQuotes 4") << u"\"1234\",\"56,78\"\n\"abcd\",\"ef,gh\"\n"_s << QList<QStringList>{{u"1234"_s, u"56,78"_s},{u"abcd"_s, u"ef,gh"_s}};

    QTest::addRow("CheckQuotedNl 1") << u"\"12\n34\",5678\n\"ab cd\",efgh\n"_s << QList<QStringList>{{u"12\n34"_s, u"5678"_s},{u"ab cd"_s, u"efgh"_s}};

    QTest::addRow("CheckEscapedQuote 1") << u"\" \"\" \",\"\"\"a\"\"\"\n"_s << QList<QStringList>{{u" \" "_s, u"\"a\""_s}};
}

void CsvDetectorTests::decodeCsvChunked()
{
    using KFileMetaData::Helper::CsvStyle;
    QFETCH(QStringList, input);
    QFETCH(QList<QStringList>, expected);

    QList<QStringList> allRows;
    QString remainder;

    for (const auto &chunk : input) {
        remainder += chunk;
        auto [rows, end] = Helper::decodeCsv(remainder, {CsvStyle::Comma, CsvStyle::NL});
        allRows.append(rows);
        if (end == remainder.size()) {
            remainder.clear();
        } else {
            allRows.removeLast(); // remove partial row
            remainder.slice(end);
        }
    }
    if (!remainder.isEmpty()) {
        auto [rows, end] = Helper::decodeCsv(remainder, {CsvStyle::Comma, CsvStyle::NL});
        allRows.append(rows);
    }

    QCOMPARE(allRows.size(), expected.size());
    for (int i = 0; i < allRows.size(); i++) {
        QCOMPARE(allRows[i], expected[i]);
    }
}

void CsvDetectorTests::decodeCsvChunked_data()
{
    QTest::addColumn<QStringList>("input");
    QTest::addColumn<QList<QStringList>>("expected");

    QTest::addRow("Single chunk")  << QStringList{u"1234,5678"_s}                     << QList<QStringList>{{u"1234"_s, u"5678"_s}};
    QTest::addRow("Two chunks")    << QStringList{u"1234"_s, u",5678"_s}              << QList<QStringList>{{u"1234"_s, u"5678"_s}};
    QTest::addRow("RowCount 1 NL") << QStringList{u"1234,"_s, u"5678\nabcd,efgh"_s}   << QList<QStringList>{{u"1234"_s, u"5678"_s},{u"abcd"_s, u"efgh"_s}};
    QTest::addRow("RowCount 2 NL") << QStringList{u"1234,"_s, u"5678\nabcd,efgh\n"_s} << QList<QStringList>{{u"1234"_s, u"5678"_s},{u"abcd"_s, u"efgh"_s}};

    QTest::addRow("Chunked Quote")      << QStringList{u"\" "_s, u"\"\" \""_s, u",\"\"\"a\"\"\"\n"_s} << QList<QStringList>{{u" \" "_s, u"\"a\""_s}};
    QTest::addRow("Chunked Quote 2 NL") << QStringList{u"\" "_s, u"\"\" \""_s, u"\n"_s, u"\"\"\"a\"\"\"\n"_s} << QList<QStringList>{{u" \" "_s},{u"\"a\""_s}};
    QTest::addRow("Chunked, NL quoted") << QStringList{u"\" "_s, u"\n"_s, u" \"\n"_s, u"\"\"\"a\"\"\"\n"_s} << QList<QStringList>{{u" \n "_s},{u"\"a\""_s}};
}

QTEST_GUILESS_MAIN(CsvDetectorTests)

#include "csvdetectortest.moc"
