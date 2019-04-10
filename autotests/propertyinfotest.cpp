/*
 * This file is part of the KDE KFileMetaData project
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "propertyinfotest.h"
#include "propertyinfo.h"

#include <QtTest>

using namespace KFileMetaData;

//QTEST_GUILESS_MAIN(PropertyInfoTest)
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    PropertyInfoTest tc;

    auto arguments = app.arguments();
    if (arguments.contains(QStringLiteral("--localized"))) {
        arguments.removeAll(QStringLiteral("--localized"));
        tc.setLocalized(true);
    }

    return QTest::qExec(&tc, arguments);
}

void PropertyInfoTest::setLocalized(bool localized)
{
    m_useLocalization = localized;
}

void PropertyInfoTest::init()
{
    if (!m_useLocalization) {
        QLocale().setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
    }
}

void PropertyInfoTest::testNameIdMapping()
{
    // The +1 is to avoid the Empty Property
    int i = static_cast<int>(Property::FirstProperty) + 1;
    int e = static_cast<int>(Property::LastProperty);

    for (; i <= e; i++) {
        Property::Property p = static_cast<Property::Property>(i);
        PropertyInfo pi(p);

        // qDebug() << pi.name();
        QCOMPARE(pi.property(), p);
        QVERIFY(!pi.name().isEmpty());
        QVERIFY(!pi.displayName().isEmpty());

        PropertyInfo pi2 = PropertyInfo::fromName(pi.name());
        QCOMPARE(pi2.property(), p);
    }
}

void PropertyInfoTest::testFormatAsDisplayString() {
    QFETCH(KFileMetaData::PropertyInfo, propertyInfo);
    QFETCH(QVariant, value);
    QFETCH(QString, expected);
    QFETCH(bool, maybeLocalized);

    if (m_useLocalization && maybeLocalized) {
        qDebug() << "Expected:" << expected << ", formatted/localized:" << propertyInfo.formatAsDisplayString(value);
        if (expected != propertyInfo.formatAsDisplayString(value)) {
            QEXPECT_FAIL("", "Expected value not localized", Continue);
        }
    }
    QCOMPARE(propertyInfo.formatAsDisplayString(value), expected);
}

void PropertyInfoTest::testFormatAsDisplayString_data()
{
    QTest::addColumn<KFileMetaData::PropertyInfo>("propertyInfo");
    QTest::addColumn<QVariant>("value");
    // expected values for an en_US locale
    QTest::addColumn<QString>("expected");
    QTest::addColumn<bool>("maybeLocalized");

    auto emptyProperty = PropertyInfo::fromName(QStringLiteral("no valid property name"));
    QTest::addRow("<invalid>")
        << emptyProperty << QVariant(QStringLiteral("empty")) << QStringLiteral("empty") << true;

    QStringList artistList = {QStringLiteral("Artist1"), QStringLiteral("Artist2"), QStringLiteral("Artist3")};
    QStringList authorList = {QStringLiteral("Author1")};
    QVariantList arrangerList = {QStringLiteral("Arranger1"), QStringLiteral("Arranger2")};
    QVariantList bitRateList = {128000, 130000};
    QVariantList titleList = {QStringLiteral("Title1"), QStringLiteral("Title2")};

    struct {
        KFileMetaData::Property::Property property;
        bool maybeLocalized;
        QVariant value;
        QString expected;
    } rows[] = {
        { Property::DiscNumber,        true,  2018,                    QStringLiteral("2018")},
        { Property::Title,             false, QStringLiteral("Title"), QStringLiteral("Title")},
        { Property::Title,             false, titleList,               QStringLiteral("Title1 and Title2")},
        { Property::Artist,            true,  artistList,              QStringLiteral("Artist1, Artist2, and Artist3")},
        { Property::Author,            true,  authorList,              QStringLiteral("Author1")},
        { Property::Arranger,          true,  arrangerList,            QStringLiteral("Arranger1 and Arranger2")},
        { Property::Duration,          true,  1800,                    QStringLiteral("0:30:00")},
        { Property::SampleRate,        true,  44100,                   QStringLiteral("44.1 kHz")},
        { Property::BitRate,           true,  128000,                  QStringLiteral("128 kbit/s")},
        { Property::BitRate,           true,  1350000,                 QStringLiteral("1.35 Mbit/s")},
        { Property::BitRate,           true,  14700000,                QStringLiteral("14.7 Mbit/s")},
        { Property::BitRate,           true,  bitRateList,             QStringLiteral("128 kbit/s and 130 kbit/s")},
        { Property::ImageOrientation,  true,  5,                       QStringLiteral("Transposed")},
        { Property::PhotoFlash,        true,  0x00,                    QStringLiteral("No flash")},
        { Property::PhotoFlash,        true,  0x50,                    QStringLiteral("No, red-eye reduction")},
        { Property::PhotoGpsAltitude,  true,  1.1,                     QStringLiteral("1.1 m")},
        // make VisualStudio compiler happy: QChar(0x00B0) = "°"
        { Property::PhotoGpsLatitude,  true,  25,                      QStringLiteral("25") + QChar(0x00B0)},
        { Property::PhotoGpsLongitude, true,  13.5,                    QStringLiteral("13.5") + QChar(0x00B0)},
        { Property::FrameRate,         true,  23,                      QStringLiteral("23 fps")},
        { Property::FrameRate,         true,  23.976,                  QStringLiteral("23.98 fps")},
        { Property::AspectRatio,       true,  1.77778,                 QStringLiteral("1.78:1")},
    };

    for (auto row : rows) {
        PropertyInfo info(row.property);
        QTest::addRow("%s", info.displayName().toUtf8().constData())
            << info << row.value << row.expected << row.maybeLocalized;
    }
}
