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

#include "externalextractortest.h"
#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "externalextractor.h"
#include "config-kfilemetadata.h"

#include <QDebug>
#include <QTest>
#include <QDir>

using namespace KFileMetaData;

QString ExternalExtractorTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void ExternalExtractorTest::test()
{
    QScopedPointer<ExtractorPlugin> plugin(
                new ExternalExtractor(
                    QStringLiteral(LIBEXEC_INSTALL_DIR) +
                    QStringLiteral("/kfilemetadata/externalextractors/pdfextractor")
    ));

    SimpleExtractionResult result(testFilePath("test.pdf"), "application/pdf");
    plugin->extract(&result);

    QCOMPARE(result.properties().value(Property::Author), QVariant(QStringLiteral("Happy Man")));
    QCOMPARE(result.properties().value(Property::Title), QVariant(QStringLiteral("The Big Brown Bear")));
    QCOMPARE(result.properties().value(Property::Subject), QVariant(QStringLiteral("PDF Metadata")));

    QString dt("D:20140701153850+02\'00\'");
    QCOMPARE(result.properties().value(Property::CreationDate), QVariant(dt));

    QCOMPARE(result.properties().size(), 4);
}

QTEST_MAIN(ExternalExtractorTest)
