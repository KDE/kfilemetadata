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
    ExternalWriter plugin{testFilePath("testexternalwriter")};
    file.open();
    WriteData data(file.fileName(), "application/text");
    plugin.write(data);

    QCOMPARE(QString(file.readAll()), QStringLiteral("{}"));
}

QTEST_GUILESS_MAIN(ExternalWriterTest)
