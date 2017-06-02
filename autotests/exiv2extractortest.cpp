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
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void Exiv2ExtractorTest::test()
{
    QScopedPointer<ExtractorPlugin> plugin(new Exiv2Extractor(this));

    SimpleExtractionResult result(testFilePath("test.jpg"), "image/jpeg");
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    using namespace KFileMetaData::Property;
    double lat = 41.4114341666667;
    double lon = 2.1736409444444;
    QCOMPARE(result.properties().value(PhotoGpsLatitude).toDouble(), lat);
    QCOMPARE(result.properties().value(PhotoGpsLongitude).toDouble(), lon);
    QCOMPARE(result.properties().value(PhotoGpsAltitude), QVariant());
}

QTEST_GUILESS_MAIN(Exiv2ExtractorTest)
