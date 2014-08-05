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

#include "epubextractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/epubextractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>
#include <KService>

using namespace KFileMetaData;

QString EPubExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void EPubExtractorTest::test()
{
    QScopedPointer<ExtractorPlugin> plugin(new EPubExtractor(this));

    SimpleExtractionResult result(testFilePath("test.epub"), "application/epub+zip");
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Document);

    // We're doing a contains instead of an exact check cause the epub file contains
    // a ton of css and other garbage.
    QVERIFY(result.text().contains(QStringLiteral("This is a sample PDF file for KFileMetaData.")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Happy Man")));
    QCOMPARE(result.properties().value(Property::Publisher), QVariant(QStringLiteral("Happy Publisher")));
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("The Big Brown Bear")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("Baloo KFileMetaData")));

    QDateTime dt(QDate(2014, 1, 1), QTime(1, 1, 1));
    dt.setTimeSpec(Qt::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 5);
}

void EPubExtractorTest::testMetaDataOnly()
{
    QScopedPointer<ExtractorPlugin> plugin(new EPubExtractor(this));

    SimpleExtractionResult result(testFilePath("test.epub"), "application/epub+zip", ExtractionResult::ExtractMetaData);
    plugin->extract(&result);

    QVERIFY(!result.types().isEmpty());
    QVERIFY(!result.properties().isEmpty());
    QVERIFY(result.text().isEmpty());
}


QTEST_MAIN(EPubExtractorTest)
