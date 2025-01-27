/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "writedata.h"
#include "indexerextractortestsconfig.h"
#include "externalwriter.h"

#include <QTest>
#include <QTemporaryFile>

class ExternalWriterTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void test();
    void testProperties();
};

using namespace KFileMetaData;

namespace {
QString pluginPath(const QString& fileName)
{
    return QLatin1String(INDEXER_TESTS_EXTERNALPLUGIN_PATH) + QLatin1Char('/') + fileName;
}
}

void ExternalWriterTest::test()
{
    QTemporaryFile file;
    ExternalWriter plugin{pluginPath(QStringLiteral("testexternalwriter"))};
    QVERIFY(file.open());
    WriteData data(file.fileName(), QStringLiteral("application/text"));
    plugin.write(data);

    QCOMPARE(file.readAll(), QByteArrayLiteral("{}"));
}

void ExternalWriterTest::testProperties()
{
    QTemporaryFile file;
    ExternalWriter plugin{pluginPath(QStringLiteral("testexternalwriter"))};
    QVERIFY(file.open());
    WriteData data(file.fileName(), QStringLiteral("application/text"));
    data.add(Property::Author, QStringLiteral("Name"));
    plugin.write(data);

    auto expected = QByteArrayLiteral("{\"author\": \"Name\"}");
    QCOMPARE(file.readAll(), expected);
}

QTEST_GUILESS_MAIN(ExternalWriterTest)

#include "externalwritertest.moc"
