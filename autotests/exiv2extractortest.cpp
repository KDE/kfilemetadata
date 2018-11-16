/*
 * <one line to give the library's name and an idea of what it does.>
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

#include "exiv2extractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/exiv2extractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>

using namespace KFileMetaData;

QString Exiv2ExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void Exiv2ExtractorTest::test()
{
    Exiv2Extractor plugin{this};

    SimpleExtractionResult result(testFilePath("test.jpg"), "image/jpeg");
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    using namespace KFileMetaData::Property;

    QCOMPARE(result.properties().value(PhotoGpsLatitude).toDouble(), 41.411);
    QCOMPARE(result.properties().value(PhotoGpsLongitude).toDouble(), 2.173);
    QVERIFY(qAbs(result.properties().value(PhotoGpsAltitude).toDouble() - 12.2) <  0.0001);
    QCOMPARE(result.properties().value(Artist).toString(), QStringLiteral("Artist"));
    QCOMPARE(result.properties().value(Description).toString(), QStringLiteral("Description"));
    QCOMPARE(result.properties().value(Copyright).toString(), QStringLiteral("Copyright"));
    QCOMPARE(result.properties().value(Generator).toString(), QStringLiteral("digiKam-5.9.0"));
}

QTEST_GUILESS_MAIN(Exiv2ExtractorTest)
