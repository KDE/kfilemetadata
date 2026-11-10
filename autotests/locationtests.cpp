/*
    SPDX-FileCopyrightText: 2026 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "../src/extractors/locationparser_p.h"

#include <QTest>

using namespace KFileMetaData;
using namespace Qt::Literals::StringLiterals;

class LocationTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testParser();
    void testParser_data();
};

void LocationTest::testParser()
{
    QFETCH(QString, locationString);
    QFETCH(float, latitude);
    QFETCH(float, longitude);
    QFETCH(std::optional<float>, altitude);

    auto res = Parser::locationFromIso6709HString(locationString);
    QVERIFY(res[0]);
    QVERIFY(res[1]);
    QCOMPARE(*res[0], latitude);
    QCOMPARE(*res[1], longitude);
    if (altitude) {
        QVERIFY(res[2]);
        QCOMPARE(*res[2], *altitude);
    }
}

void LocationTest::testParser_data()
{
    QTest::addColumn<QString>("locationString");
    QTest::addColumn<float>("latitude");
    QTest::addColumn<float>("longitude");
    QTest::addColumn<std::optional<float>>("altitude");

    // clang-format off
    QTest::addRow("NW_no_alt")          << u"+42.3630-071.0602/"_s    << +42.3630f << -71.0602f << std::optional<float>{};
    QTest::addRow("NW_no_alt_comma")    << u"+42,3630-071,0602/"_s    << +42.3630f << -71.0602f << std::optional<float>{};
    QTest::addRow("SE_no_alt")          << u"-42.3630+171.0602/"_s    << -42.3630f << 171.0602f << std::optional<float>{};
    QTest::addRow("SE_no_alt_integral") << u"-42+071/"_s              << -42.0000f << +71.0000f << std::optional<float>{};
    QTest::addRow("SW_no_alt_integral") << u"-42-071/"_s              << -42.0000f << -71.0000f << std::optional<float>{};
    QTest::addRow("NE_w_alt")           << u"+42.01+071.02+100.03/"_s << +42.0100f << +71.0200f << std::optional<float>{100.03f};
    QTest::addRow("NE_w_alt_integral")  << u"+42+071+100/"_s          << +42.0000f << +71.0000f << std::optional<float>{100.0f};
    QTest::addRow("MountEverest")       << u"+27.5916+086.5640+8850CRSWGS_84/"_s << +27.5916f << +86.5640f << std::optional<float>{8850.0f};
    // clang-format on
}

QTEST_GUILESS_MAIN(LocationTest)

#include "locationtests.moc"
