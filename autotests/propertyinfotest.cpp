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
    QCOMPARE(emptyProperty.formatAsDisplayString(QVariant("empty")), QStringLiteral("empty"));

    PropertyInfo year(Property::DiscNumber);
    QCOMPARE(year.formatAsDisplayString(QVariant(2018)), QStringLiteral("2018"));

    QStringList artistList = {"Artist1", "Artist2"};
    PropertyInfo artist(Property::Artist);
    QCOMPARE(artist.formatAsDisplayString(QVariant(artistList)), QStringLiteral("Artist1, Artist2"));

    QStringList authorList = {"Author1"};
    PropertyInfo author(Property::Author);
    QCOMPARE(artist.formatAsDisplayString(QVariant(authorList)), QStringLiteral("Author1"));

    PropertyInfo duration(Property::Duration);
    QCOMPARE(duration.formatAsDisplayString(QVariant(1800)), QStringLiteral("0:30:00"));

    PropertyInfo sampleRate(Property::SampleRate);
    QCOMPARE(sampleRate.formatAsDisplayString(QVariant(44100)), QString(QLocale().toString(44.1) + QStringLiteral(" kHz")));

    PropertyInfo bitRate(Property::BitRate);
    QCOMPARE(bitRate.formatAsDisplayString(QVariant(128000)), QStringLiteral("128 kB/s"));

    PropertyInfo orientation(Property::ImageOrientation);
    QCOMPARE(orientation.formatAsDisplayString(QVariant(5)), QStringLiteral("Transposed"));

    PropertyInfo altitude(Property::PhotoGpsAltitude);
    QCOMPARE(altitude.formatAsDisplayString(QVariant(1.1)), QString(QLocale().toString(1.1)  + QStringLiteral(" m")));

    PropertyInfo latitude(Property::PhotoGpsLatitude);
    QCOMPARE(latitude.formatAsDisplayString(QVariant(25)), QString(QLocale().toString(25)  + QStringLiteral("°")));

    PropertyInfo longitude(Property::PhotoGpsLongitude);
    QCOMPARE(longitude.formatAsDisplayString(QVariant(13.5)), QString(QLocale().toString(13.5)  + QStringLiteral("°")));

    PropertyInfo framerate(Property::FrameRate);
    QCOMPARE(framerate.formatAsDisplayString(QVariant(23)), QStringLiteral("23 fps"));
}

QTEST_GUILESS_MAIN(PropertyInfoTest)
