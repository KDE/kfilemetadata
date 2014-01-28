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
#include <qtest_kde.h>

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

QTEST_KDEMAIN_CORE(PropertyInfoTest)
