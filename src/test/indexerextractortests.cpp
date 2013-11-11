/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2013 David Edmundson <davidedmundson@kde.org>

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

#include "indexerextractortests.h"

#include <Soprano/Vocabulary/RDF>

#include <QtTest>

#include "simpleresourcegraph.h"
#include "qtest_kde.h"
#include "nie.h"
#include "nfo.h"

#include "../plaintextextractor.h"

#include "indexerextractortestsconfig.h"

using namespace Nepomuk2::Vocabulary;
using namespace Soprano::Vocabulary;

IndexerExtractorTests::IndexerExtractorTests(QObject* parent) :
    QObject(parent)
{
}

QUrl IndexerExtractorTests::testFilePath(const QString& fileName) const
{
    return QUrl(QString(NEPOMUK_INDEXER_TESTS_SAMPLE_FILES_PATH) + '/' + fileName);
}

void IndexerExtractorTests::benchMarkPlainTextExtractor()
{
    QScopedPointer<Nepomuk2::ExtractorPlugin> plugin(new Nepomuk2::PlainTextExtractor(this, QVariantList()));
    QBENCHMARK(plugin->extract(QUrl(), testFilePath("plain_text_file.txt"), "text/plain"));
}

void IndexerExtractorTests::testPlainTextExtractor()
{
    QScopedPointer<Nepomuk2::ExtractorPlugin> plugin(new Nepomuk2::PlainTextExtractor(this, QVariantList()));
    QUrl resUri("nepomuk://a");

    const Nepomuk2::SimpleResourceGraph actualResourceGraph = plugin->extract(resUri, testFilePath("plain_text_file.txt"), "text/plain");

    QString content;
    QTextStream(&content) << "This is a text file\n"
                          << "it is four lines long\n"
                          << "it has 77 characters\n"
                          << "and 17 words.\n";

    const Nepomuk2::SimpleResource fileResource = actualResourceGraph[resUri];

    QVERIFY(fileResource.isValid());
    QVERIFY(fileResource.property(RDF::type()) == QVariantList() << NFO::PlainTextDocument());
    QVERIFY(fileResource.property(NIE::plainTextContent()) ==  QVariantList() << content);
    QVERIFY(fileResource.property(NFO::wordCount()) ==  QVariantList() << 17);
    QVERIFY(fileResource.property(NFO::characterCount()) ==  QVariantList() << 77);
    QVERIFY(fileResource.property(NFO::lineCount()) ==  QVariantList() << 4);
}

QTEST_KDEMAIN_CORE(IndexerExtractorTests)

