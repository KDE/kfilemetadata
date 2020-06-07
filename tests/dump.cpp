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
    parser.addPositionalArgument(QStringLiteral("filename"), QStringLiteral("File to process"));
    parser.process(app);

    if (parser.positionalArguments().size() != 1) {
        qDebug() << "Only one argument is accepted";
        parser.showHelp(1);
    }

    QString url = QFileInfo(parser.positionalArguments().at(0)).absoluteFilePath();

    QMimeDatabase mimeDb;
    QString mimetype = mimeDb.mimeTypeForFile(url).name();

    KFileMetaData::ExtractorCollection extractors;
    QList<KFileMetaData::Extractor*> exList = extractors.fetchExtractors(mimetype);

    QTextStream out(stdout);
    out << url << " " << mimetype << "\n\n";

    for (KFileMetaData::Extractor* ex : qAsConst(exList)) {
        KFileMetaData::SimpleExtractionResult result(url, mimetype,
                                                     KFileMetaData::ExtractionResult::ExtractMetaData);
        ex->extract(&result);

        out << "Extractor For: " << ex->mimetypes().join(QLatin1Char(' ')) << "\n";
        KFileMetaData::PropertyMap properties = result.properties();
        KFileMetaData::PropertyMap::const_iterator it = properties.constBegin();
        for (; it != properties.constEnd(); it++) {
            out << KFileMetaData::PropertyInfo(it.key()).displayName() << " --> "
                << it.value().toString() << " (" << it.value().typeName() << ")\n";
        }
        out << "-------------\n";
    }

    return 0;
}
