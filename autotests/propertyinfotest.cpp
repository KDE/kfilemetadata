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

void PropertyInfoTest::testFormatAsDisplayString()
{
    auto emptyProperty = PropertyInfo::fromName(QStringLiteral("no valid property name"));
    QCOMPARE(emptyProperty.formatAsDisplayString(QVariant(QStringLiteral("empty"))), QStringLiteral("empty"));

    PropertyInfo year(Property::DiscNumber);
    QCOMPARE(year.formatAsDisplayString(QVariant(2018)), QStringLiteral("2018"));

    PropertyInfo title(Property::Title);
    QCOMPARE(title.formatAsDisplayString(QVariant(QStringLiteral("Title"))), QStringLiteral("Title"));

    QStringList artistList = {QStringLiteral("Artist1"), QStringLiteral("Artist2")};
    PropertyInfo artist(Property::Artist);
    QCOMPARE(artist.formatAsDisplayString(QVariant(artistList)), QStringLiteral("Artist1, Artist2"));

    QStringList authorList = {QStringLiteral("Author1")};
    PropertyInfo author(Property::Author);
    QCOMPARE(author.formatAsDisplayString(QVariant(authorList)), QStringLiteral("Author1"));

    PropertyInfo duration(Property::Duration);
    QCOMPARE(duration.formatAsDisplayString(QVariant(1800)), QStringLiteral("0:30:00"));

    PropertyInfo sampleRate(Property::SampleRate);
    QCOMPARE(sampleRate.formatAsDisplayString(QVariant(44100)), QString(QLocale().toString(44.1) + QStringLiteral(" kHz")));

    PropertyInfo bitRate(Property::BitRate);
    QCOMPARE(bitRate.formatAsDisplayString(QVariant(128000)), QStringLiteral("128 kbit/s"));
    QCOMPARE(bitRate.formatAsDisplayString(QVariant(1350000)), QString(QLocale().toString(1.35) + QStringLiteral(" Mbit/s")));
    QCOMPARE(bitRate.formatAsDisplayString(QVariant(14700000)), QString(QLocale().toString(14.7) + QStringLiteral(" Mbit/s")));

    PropertyInfo orientation(Property::ImageOrientation);
    QCOMPARE(orientation.formatAsDisplayString(QVariant(5)), QStringLiteral("Transposed"));

    PropertyInfo flash(Property::PhotoFlash);
    QCOMPARE(flash.formatAsDisplayString(QVariant(0x00)), QStringLiteral("No flash"));
    QCOMPARE(flash.formatAsDisplayString(QVariant(0x50)), QStringLiteral("No, red-eye reduction"));

    PropertyInfo altitude(Property::PhotoGpsAltitude);
    QCOMPARE(altitude.formatAsDisplayString(QVariant(1.1)), QString(QLocale().toString(1.1) + QStringLiteral(" m")));

    PropertyInfo latitude(Property::PhotoGpsLatitude);
    // make tests on windows happy: QChar(0x00B0) = "°"
    QCOMPARE(latitude.formatAsDisplayString(QVariant(25)), QString(QLocale().toString(25) + QChar(0x00B0)));

    PropertyInfo longitude(Property::PhotoGpsLongitude);
    QCOMPARE(longitude.formatAsDisplayString(QVariant(13.5)), QString(QLocale().toString(13.5) + QChar(0x00B0)));

    PropertyInfo framerate(Property::FrameRate);
    QCOMPARE(framerate.formatAsDisplayString(QVariant(23)), QStringLiteral("23 fps"));
}

QTEST_GUILESS_MAIN(PropertyInfoTest)
