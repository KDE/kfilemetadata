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

#include "odfextractortest.h"

#include <QTest>
#include <QDir>
#include <QDebug>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/odfextractor.h"

using namespace KFileMetaData;

QString OdfExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void OdfExtractorTest::testText()
{
    QScopedPointer<ExtractorPlugin> plugin(new OdfExtractor(this));

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.odt")), QStringLiteral("application/vnd.oasis.opendocument.text"));
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Document);

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("KFileMetaData Subject")));
    QCOMPARE(result.properties().value(Property::Keywords), QVariant(QStringLiteral("KFileMetaData keyword")));
    QCOMPARE(result.properties().value(Property::Comment), QVariant(QStringLiteral("KFileMetaData comment")));
    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));

    QDateTime dt(QDate(2014, 07, 01), QTime(17, 37, 40, 690));
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().value(Property::WordCount), QVariant(4));
    QCOMPARE(result.properties().value(Property::PageCount), QVariant(1));
    QCOMPARE(result.text(), QStringLiteral("Test file for KFileMetaData. "));
    QCOMPARE(result.properties().size(), 8);
}

void OdfExtractorTest::testTextMetaDataOnly()
{
    QScopedPointer<ExtractorPlugin> plugin(new OdfExtractor(this));

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.odt")), QStringLiteral("application/vnd.oasis.opendocument.text"), ExtractionResult::ExtractMetaData);
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.properties().size(), 8);
    QVERIFY(result.text().isEmpty());
}

void OdfExtractorTest::testPresentation()
{
    QScopedPointer<ExtractorPlugin> plugin(new OdfExtractor(this));

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.odp")), QStringLiteral("application/vnd.oasis.opendocument.presentation"));
    plugin->extract(&result);

    QCOMPARE(result.types().size(), 2);
    QCOMPARE(result.types().at(0), Type::Document);
    QCOMPARE(result.types().at(1), Type::Presentation);

    QVERIFY(result.properties().value(Property::Generator).toString().contains(QStringLiteral("LibreOffice")));
    QDateTime dt(QDate(2014, 07, 02), QTime(10, 59, 23, 434));
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.text(), QStringLiteral("KFileMetaData Pres "));
}


QTEST_MAIN(OdfExtractorTest)
