/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QTextStream>

#include "extractor.h"
#include "extractorcollection.h"
#include "propertyinfo.h"
#include "simpleextractionresult.h"

#include <iostream>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addOption({{"f", "fulltext"}, "Extract full text"});
    parser.addPositionalArgument(QStringLiteral("filename"), QStringLiteral("File to process"));
    parser.process(app);

    if (parser.positionalArguments().size() != 1) {
        qDebug() << "Exactly one argument is accepted";
        parser.showHelp(1);
    }
    bool extractFulltext = parser.isSet("fulltext");

    using KFileMetaData::ExtractionResult;
    const ExtractionResult::Flags extractionLevel = (extractFulltext
        ? ExtractionResult::ExtractMetaData | ExtractionResult::ExtractPlainText
        : ExtractionResult::ExtractMetaData);

    QString url = QFileInfo(parser.positionalArguments().at(0)).absoluteFilePath();

    QMimeDatabase mimeDb;
    QString mimetype = mimeDb.mimeTypeForFile(url).name();

    KFileMetaData::ExtractorCollection extractors;
    QList<KFileMetaData::Extractor*> exList = extractors.fetchExtractors(mimetype);

    QTextStream out(stdout);
    out << url << " " << mimetype << "\n";

    for (KFileMetaData::Extractor* ex : std::as_const(exList)) {
        const QString extractorName = ex->extractorProperties().value(QStringLiteral("Name")).toString();
        out << "\t";
        if (!extractorName.isEmpty()) {
            out << extractorName;
        } else {
            out << "Extractor";
        }
        out << " For " << ex->mimetypes().join(QLatin1Char(' ')) << "\n";

        KFileMetaData::SimpleExtractionResult result(url, mimetype, extractionLevel);
        ex->extract(&result);

        const KFileMetaData::PropertyMultiMap multiMap= result.properties();
        KFileMetaData::PropertyMultiMap::const_iterator it = multiMap.constBegin();
        for (; it != multiMap.constEnd(); it++) {
            out << "\t\t" << KFileMetaData::PropertyInfo(it.key()).displayName() << ": "
                << it.value().toString() << " (" << it.value().typeName() << ")\n";
        }
        if (extractFulltext) {
            out << "\t\tText: " << result.text() << "\n";
        }
        out << "\t-------------\n";
    }

    return 0;
}
