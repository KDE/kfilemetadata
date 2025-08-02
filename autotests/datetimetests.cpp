/*
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "../src/datetimeparser_p.h"

#include <QTest>
#include <QTimeZone>

using namespace KFileMetaData;
using namespace Qt::Literals::StringLiterals;

class DateTimeTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testParser();
    void testParser_data();
    void testExifParser();
    void testExifParser_data();
};

void DateTimeTest::testParser()
{
    QFETCH(QString, timeString);
    QFETCH(QDateTime, expected);

    auto dt = Parser::dateTimeFromString(timeString);

    QCOMPARE(dt, expected);
}

void DateTimeTest::testParser_data()
{
    QTest::addColumn<QString>("timeString");
    QTest::addColumn<QDateTime>("expected");

    // clang-format off
    QTest::addRow("ISO-8601-noTZ") << u"2025-07-27T14:35:56.448986511"_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 449), QTimeZone::LocalTime);
    QTest::addRow("ISO-8601")      << u"2025-07-27T14:35:56.448986511+02:00"_s << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 449), QTimeZone(7200));
    QTest::addRow("ISO-date-only") << u"2025-07-27"_s                          << QDateTime(QDate(2025, 07, 27), QTime(0, 0, 0), QTimeZone::LocalTime);
    // clang-format on
}

void DateTimeTest::testExifParser()
{
    QFETCH(QString, timeString);
    QFETCH(QString, offsetString);
    QFETCH(QString, subSecString);
    QFETCH(QDateTime, expected);

    auto dt = Parser::dateTimeFromExifString(timeString, subSecString, offsetString, {});

    QCOMPARE(dt, expected);
}

void DateTimeTest::testExifParser_data()
{
    QTest::addColumn<QString>("timeString");
    QTest::addColumn<QString>("subSecString");
    QTest::addColumn<QString>("offsetString");
    QTest::addColumn<QDateTime>("expected");

    // clang-format off
    QTest::addRow("noTZ")             << u"2025:07:27 14:35:56"_s << u""_s       << u""_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56), QTimeZone::LocalTime);
    QTest::addRow("ISO-TZ")           << u"2025:07:27 14:35:56"_s << u""_s       << u"+02:00"_s << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56), QTimeZone(7200));
    QTest::addRow("Subsec-1")         << u"2025:07:27 14:35:56"_s << u"1"_s      << u""_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 100), QTimeZone::LocalTime);
    QTest::addRow("Subsec-200")       << u"2025:07:27 14:35:56"_s << u"200"_s    << u""_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 200), QTimeZone::LocalTime);
    QTest::addRow("Subsec-trim")      << u"2025:07:27 14:35:56"_s << u"34 "_s    << u""_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 340), QTimeZone::LocalTime);
    QTest::addRow("Subsec-rounddown") << u"2025:07:27 14:35:56"_s << u"34544 "_s << u""_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 345), QTimeZone::LocalTime);
    QTest::addRow("Subsec-roundup")   << u"2025:07:27 14:35:56"_s << u"34567"_s  << u""_s       << QDateTime(QDate(2025, 07, 27), QTime(14, 35, 56, 346), QTimeZone::LocalTime);
    // clang-format on
}

QTEST_GUILESS_MAIN(DateTimeTest)

#include "datetimetests.moc"
