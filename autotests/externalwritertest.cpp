/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "externalwritertest.h"
#include "writedata.h"
#include "indexerextractortestsconfig.h"
#include "externalwriter.h"
#include "config-kfilemetadata.h"

#include <QTest>
#include <QTemporaryFile>

using namespace KFileMetaData;

QString ExternalWriterTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_CONFIGURED_FILES_PATH) + QLatin1Char('/') + fileName;
}

void ExternalWriterTest::test()
{
    QTemporaryFile file;
    ExternalWriter plugin{testFilePath(QStringLiteral("testexternalwriter"))};
    QVERIFY(file.open());
    WriteData data(file.fileName(), QStringLiteral("application/text"));
    plugin.write(data);

    QCOMPARE(file.readAll(), QByteArrayLiteral("{}"));
}

void ExternalWriterTest::testProperties()
{
    QTemporaryFile file;
    ExternalWriter plugin{testFilePath(QStringLiteral("testexternalwriter"))};
    QVERIFY(file.open());
    WriteData data(file.fileName(), QStringLiteral("application/text"));
    data.add(Property::Author, QStringLiteral("Name"));
    plugin.write(data);

    auto expected = QByteArrayLiteral("{\"author\": \"Name\"}");
    QCOMPARE(file.readAll(), expected);
}

QTEST_GUILESS_MAIN(ExternalWriterTest)

#include "moc_externalwritertest.cpp"
