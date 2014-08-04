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

#include "popplerextractortest.h"
#include "simpleresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/popplerextractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>
#include <KService>

using namespace KFileMetaData;

QString PopplerExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void PopplerExtractorTest::test()
{
    QScopedPointer<ExtractorPlugin> plugin(new PopplerExtractor(this, QVariantList()));

    SimpleResult result(testFilePath("test.pdf"), "application/pdf");
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Document);

    QCOMPARE(result.text(), QStringLiteral("This is a sample PDF file for KFileMetaData. "));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Happy Man")));
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("The Big Brown Bear")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("PDF Metadata")));
    QCOMPARE(result.properties().value(Property::Generator), QVariant(QStringLiteral("LibreOffice 4.2")));

    QDateTime dt(QDate(2014, 07, 01), QTime(13, 38, 50));
    dt.setTimeSpec(Qt::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 5);
}

void PopplerExtractorTest::testMetaDataOnly()
{
    QScopedPointer<ExtractorPlugin> plugin(new PopplerExtractor(this, QVariantList()));

    SimpleResult result(testFilePath("test.pdf"), "application/pdf", ExtractionResult::ExtractMetaData);
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QVERIFY(result.text().isEmpty());
    QCOMPARE(result.properties().size(), 5);

}

QTEST_MAIN(PopplerExtractorTest)
