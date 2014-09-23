/*
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QTextStream>

#include "extractorplugin.h"
#include "extractorcollection.h"
#include "propertyinfo.h"
#include "simpleextractionresult.h"

#include <iostream>

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addPositionalArgument("filename", "File to process");
    parser.process(app);

    if (parser.positionalArguments().size() != 1) {
        qDebug() << "Only one arugment is accepted";
        parser.showHelp(1);
    }

    QString url = QFileInfo(parser.positionalArguments().first()).absoluteFilePath();

    QMimeDatabase mimeDb;
    QString mimetype = mimeDb.mimeTypeForFile(url).name();

    KFileMetaData::ExtractorCollection extractors;
    QList<KFileMetaData::ExtractorPlugin*> exList = extractors.fetchExtractors(mimetype);

    QTextStream out(stdout);
    out << url << " " << mimetype << "\n\n";

    Q_FOREACH (KFileMetaData::ExtractorPlugin* ex, exList) {
        KFileMetaData::SimpleExtractionResult result(url, mimetype,
                                                     KFileMetaData::ExtractionResult::ExtractMetaData);
        ex->extract(&result);

        out << "Extractor For: " << ex->mimetypes().join(" ") << "\n";
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
