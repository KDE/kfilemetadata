/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "mobiextractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/mobiextractor.h"
#include "mimeutils.h"

#include <QTest>
#include <QMimeDatabase>
#include <QMimeType>

using namespace KFileMetaData;
using namespace Qt::StringLiterals;

QString MobiExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void MobiExtractorTest::test()
{
    MobiExtractor plugin{this};

    QString fileName = testFilePath(u"test.mobi"_s);
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().first(), Type::Document);

    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Happy Man")));
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("The Big Brown Bear")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("Baloo KFileMetaData")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("Honey")));
    QCOMPARE(result.properties().value(Property::Copyright), QVariant(QStringLiteral("License")));
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(QDateTime(QDate(2014, 01, 01), QTime(01, 1, 1), QTimeZone::UTC)));

    QCOMPARE(result.properties().size(), 5);
}

QTEST_GUILESS_MAIN(MobiExtractorTest)

#include "moc_mobiextractortest.cpp"
