/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "externalwritertest.h"
#include "config-kfilemetadata.h"
#include "externalwriter.h"
#include "indexerextractortestsconfig.h"
#include "writedata.h"

#include <QTemporaryFile>
#include <QTest>

using namespace KFileMetaData;

QString ExternalWriterTest::testFilePath(const QString &fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_CONFIGURED_FILES_PATH) + QLatin1Char('/') + fileName;
}

void ExternalWriterTest::test()
{
    QTemporaryFile file;
    ExternalWriter plugin{testFilePath("testexternalwriter")};
    file.open();
    WriteData data(file.fileName(), "application/text");
    plugin.write(data);

    QCOMPARE(QString(file.readAll()), QStringLiteral("{}"));
}

QTEST_GUILESS_MAIN(ExternalWriterTest)
