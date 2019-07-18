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
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/popplerextractor.h"
#include "mimeutils.h"

#include <QTest>
#include <QMimeDatabase>

using namespace KFileMetaData;

QString PopplerExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void PopplerExtractorTest::test()
{
    PopplerExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test.pdf"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().constFirst(), Type::Document);

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
    PopplerExtractor plugin{this};

    SimpleExtractionResult result(testFilePath("test.pdf"), "application/pdf", ExtractionResult::ExtractMetaData);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QVERIFY(result.text().isEmpty());
    QCOMPARE(result.properties().size(), 5);

}

QTEST_GUILESS_MAIN(PopplerExtractorTest)
