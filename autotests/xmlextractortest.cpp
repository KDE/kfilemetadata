/*
    SPDX-FileCopyrightText: 2018 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "xmlextractortest.h"

#include <QTest>
#include <QTemporaryFile>
#include <QMimeDatabase>

#include "simpleextractionresult.h"
#include "indexerextractortestsconfig.h"
#include "extractors/xmlextractor.h"
#include "mimeutils.h"

using namespace KFileMetaData;

XmlExtractorTests::XmlExtractorTests(QObject* parent) :
    QObject(parent)
{
}

QString XmlExtractorTests::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void XmlExtractorTests::testNoExtraction()
{
    XmlExtractor plugin{this};

    QString fileName = testFilePath(QStringLiteral("test_with_metadata.svg"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType,
            ExtractionResult::ExtractNothing);
    plugin.extract(&result);

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Image);
    QCOMPARE(result.properties().size(), 0);
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

    QString fileName = testFilePath(QStringLiteral("test_with_metadata.svg"));
    QMimeDatabase mimeDb;
    QString mimeType = MimeUtils::strictMimeType(fileName, mimeDb).name();
    QVERIFY(plugin.mimetypes().contains(mimeType));

    SimpleExtractionResult result(fileName, mimeType,
            ExtractionResult::ExtractMetaData | ExtractionResult::ExtractPlainText);
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
            ExtractionResult::ExtractMetaData | ExtractionResult::ExtractPlainText);
    plugin.extract(&result);

    QString content = QStringLiteral("Some text below <a>\n");

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Image);

    QCOMPARE(result.properties().size(), 0);

    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text(), content);
}

void XmlExtractorTests::testXmlExtractorMathML()
{
    XmlExtractor plugin{this};

    SimpleExtractionResult result(testFilePath(QStringLiteral("test.mml")),
            QStringLiteral("application/mathml+xml"),
            ExtractionResult::ExtractMetaData | ExtractionResult::ExtractPlainText);
    plugin.extract(&result);

    QString content = QStringLiteral("1 + 1 = 2\n");

    QCOMPARE(result.types().size(), 1);
    QCOMPARE(result.types().at(0), Type::Text);

    QCOMPARE(result.properties().size(), 0);

    content.replace(QLatin1Char('\n'), QLatin1Char(' '));
    QCOMPARE(result.text(), content);
}


QTEST_GUILESS_MAIN(XmlExtractorTests)

