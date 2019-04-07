/*
 * Copyright (C) 2018  Stefan Brüns <stefan.bruens@rwth-aachen.de>
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

#include "postscriptdscextractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/postscriptdscextractor.h"
#include "mimeutils.h"

#include <QTest>

using namespace KFileMetaData;

QString PostscriptDscExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void PostscriptDscExtractorTest::testPS()
{
    DscExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.ps"));
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Document);

    QCOMPARE(result.properties().value(Property::Title).toString(), QStringLiteral("The Big Brown Bear"));
    QCOMPARE(result.properties().value(Property::PageCount).toInt(), 2);

    QDateTime dt(QDate(2018, 10, 28), QTime(21, 13, 39));
    dt.setOffsetFromUtc(+3600);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 3);
}

void PostscriptDscExtractorTest::testEPS()
{
    DscExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.eps"));
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Image);

    QCOMPARE(result.properties().value(Property::Title).toString(), QStringLiteral("The Big Brown Bear"));
    QCOMPARE(result.properties().value(Property::PageCount).toInt(), 1);

    QDateTime dt(QDate(2018, 10, 28), QTime(21, 13, 39));
    dt.setOffsetFromUtc(-5400);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));
    QCOMPARE(result.properties().size(), 3);
}

QTEST_GUILESS_MAIN(PostscriptDscExtractorTest)
