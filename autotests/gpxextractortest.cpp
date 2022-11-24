/*
    SPDX-FileCopyrightText: 2022 Kai Uwe Broulik <kde@broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "gpxextractortest.h"
#include "extractors/gpxextractor.h"
#include "indexerextractortestsconfig.h"
#include "mimeutils.h"
#include "simpleextractionresult.h"

#include <QMimeDatabase>
#include <QTest>

using namespace KFileMetaData;

void GpxExtractorTest::test()
{
    GpxExtractor plugin{this};

    const QString path = QStringLiteral(INDEXER_TESTS_SAMPLE_FILES_PATH "/test.gpx");
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(path, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(path, mimeType);
    plugin.extract(&result);

    QVERIFY(result.types().isEmpty());
    QVERIFY(result.text().isEmpty());

    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("KFileMetaData Title")));
    QCOMPARE(result.properties().value(Property::Description), QVariant(QStringLiteral("KFileMetaData Description")));
    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("komoot")));

    QDateTime dt(QDate(2022, 11, 24), QTime(16, 52, 55, 34));
    dt.setTimeSpec(Qt::UTC);
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 4);
}

QTEST_GUILESS_MAIN(GpxExtractorTest)
