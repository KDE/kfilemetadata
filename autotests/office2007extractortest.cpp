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

#include "office2007extractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/office2007extractor.h"

#include <QDebug>
#include <QTest>
#include <QDir>
#include <KService>

using namespace KFileMetaData;

QString Office2007ExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void Office2007ExtractorTest::test()
{
    QScopedPointer<ExtractorPlugin> plugin(new Office2007Extractor(this, QVariantList()));

    SimpleExtractionResult result(testFilePath("test_libreoffice.docx"), "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Document);

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Comment), QVariant(QStringLiteral("KFileMetaData comment")));
    QCOMPARE(result.properties().value(Property::Langauge), QVariant(QStringLiteral("en-US")));
    QVERIFY(result.properties().value(Property::Generator).toString().contains("LibreOffice"));

    QDateTime dt(QDate(2014, 07, 01), QTime(17, 37, 40));
    dt.setTimeSpec(Qt::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 7);
    QCOMPARE(result.text(), QStringLiteral("Test file for KFileMetaData. "));
}

void Office2007ExtractorTest::testMetaDataOnly()
{
    QScopedPointer<ExtractorPlugin> plugin(new Office2007Extractor(this, QVariantList()));

    SimpleExtractionResult result(testFilePath("test_libreoffice.docx"),
                        "application/vnd.openxmlformats-officedocument.wordprocessingml.document",
                        ExtractionResult::ExtractMetaData);

    plugin->extract(&result);

    QVERIFY(!result.types().isEmpty());
    QVERIFY(!result.properties().isEmpty());
    QVERIFY(result.text().isEmpty());
}

QTEST_MAIN(Office2007ExtractorTest)
