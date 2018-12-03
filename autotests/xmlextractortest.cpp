/*
   Copyright (C) 2018  Stefan Brüns <stefan.bruens@rwth-aachen.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xmlextractortest.h"

#include <QtTest>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/xmlextractor.h"

using namespace KFileMetaData;

XmlExtractorTests::XmlExtractorTests(QObject* parent) :
    QObject(parent)
{
}

QString XmlExtractorTests::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void XmlExtractorTests::benchMarkXmlExtractor()
{
    XmlExtractor plugin(this);

    // generate a test file with varying number of words per line
    QTemporaryFile file(QStringLiteral("XXXXXX.xml"));
    QVERIFY(file.open());

    int count = 0;
    file.write("<xml>\n");
    QByteArray chunk("<text>foo bar </text>");
    for (int line = 0; line < 10000; ++line) {
        // staircase pattern, 0, 1, 2, ... 98, 0, 0, 1 ... chunks per line
        for (int i = 0; i < line % 100; ++i) {
            count++;
            file.write(chunk);
        }
        file.write("\n");
    }
    file.write("</xml>\n");
    file.close();

    SimpleExtractionResult result(file.fileName(), QStringLiteral("application/xml"));

    plugin.extract(&result);

    QString content = QStringLiteral("foo bar\n");
    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text().leftRef(8), content.leftRef(8));
    QCOMPARE(result.text().size(), 8 * count);

    QBENCHMARK {
        plugin.extract(&result);
    }
}

void XmlExtractorTests::testXmlExtractor()
{
    XmlExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_with_metadata.svg")),
            QStringLiteral("image/svg"),
            ExtractionResult::ExtractEverything);
    plugin.extract(&result);

    QString content = QStringLiteral("Some text\n");

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Image);

    QCOMPARE(result.properties().size(), 1);
    QCOMPARE(result.properties().value(Property::Title).toString(), QStringLiteral("Document Title"));

    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text(), content);
}

void XmlExtractorTests::testXmlExtractorNoContent()
{
    XmlExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_with_metadata.svg")),
            QStringLiteral("image/svg"),
            ExtractionResult::ExtractMetaData);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Image);

    QCOMPARE(result.properties().size(), 1);
    QCOMPARE(result.properties().value(Property::Title).toString(), QStringLiteral("Document Title"));

    QVERIFY(result.text().isEmpty());
}

void XmlExtractorTests::testXmlExtractorContainer()
{
    XmlExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test_with_container.svg")),
            QStringLiteral("image/svg"),
            ExtractionResult::ExtractEverything);
    plugin.extract(&result);

    QString content = QStringLiteral("Some text below <a>\n");

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Image);

    QCOMPARE(result.properties().size(), 0);

    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text(), content);
}

QTEST_GUILESS_MAIN(XmlExtractorTests)

