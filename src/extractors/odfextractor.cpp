/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Vishesh Handa <me@vhanda.in>
    Copyright (C) 2012  Jörg Ehrichs <joerg.ehrichs@gmx.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "odfextractor.h"

#include <KDE/KDebug>
#include <KDE/KZip>

#include <QDomDocument>
#include <QXmlStreamReader>

using namespace KFileMetaData;

OdfExtractor::OdfExtractor(QObject* parent, const QVariantList&): ExtractorPlugin(parent)
{

}

QStringList OdfExtractor::mimetypes()
{
    QStringList list;
    list << QLatin1String("application/vnd.oasis.opendocument.text")
         << QLatin1String("application/vnd.oasis.opendocument.presentation")
         << QLatin1String("application/vnd.oasis.opendocument.spreadsheet");

    return list;
}

void OdfExtractor::extract(ExtractionResult* result)
{
    KZip zip(result->inputUrl());
    if (!zip.open(QIODevice::ReadOnly)) {
        qWarning() << "Document is not a valid ZIP archive";
        return;
    }

    const KArchiveDirectory* directory = zip.directory();
    if (!directory) {
        qWarning() << "Invalid document structure (main directory is missing)";
        return;
    }

    const QStringList entries = directory->entries();
    if (!entries.contains("meta.xml")) {
        qWarning() << "Invalid document structure (meta.xml is missing)";
        return;
    }

    QDomDocument metaData("metaData");
    const KArchiveFile* file = static_cast<const KArchiveFile*>(directory->entry("meta.xml"));
    metaData.setContent(file->data());

    // parse metadata ...
    QDomElement docElem = metaData.documentElement();

    QDomNode n = docElem.firstChild().firstChild(); // <office:document-meta> ... <office:meta> ... content
    while (!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {
            const QString tagName = e.tagName();

            // Dublin Core
            if (tagName == QLatin1String("dc:description")) {
                result->add("dc:description", e.text());
            } else if (tagName == QLatin1String("dc:subject")) {
                result->add("dc:subject", e.text());
            } else if (tagName == QLatin1String("dc:title")) {
                result->add("dc:title", e.text());
            } else if (tagName == QLatin1String("dc:creator")) {
                result->add("dc:creator", e.text());
            } else if (tagName == QLatin1String("dc:langauge")) {
                result->add("dc:language", e.text());
            }

            // Meta Properties
            else if (tagName == QLatin1String("meta:document-statistic")) {
                bool ok = false;
                int pageCount = e.attribute("meta:page-count").toInt(&ok);
                if (ok) {
                    result->add("pageCount", pageCount);
                }

                int wordCount = e.attribute("meta:word-count").toInt(&ok);
                if (ok) {
                    result->add("wordCount", wordCount);
                }
            } else if (tagName == QLatin1String("meta:keyword")) {
                QString keywords = e.text();
                    result->add("keyword", keywords);
            } else if (tagName == QLatin1String("meta:generator")) {
                result->add("generator", e.text());
            } else if (tagName == QLatin1String("meta:creation-date")) {
                QDateTime dt = ExtractorPlugin::dateTimeFromString(e.text());
                if (!dt.isNull())
                    result->add("contentCreated", dt);
            }
        }
        n = n.nextSibling();
    }

    const KArchiveFile* contentsFile = static_cast<const KArchiveFile*>(directory->entry("content.xml"));
    QXmlStreamReader xml(contentsFile->createDevice());

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isCharacters()) {
            QString str = xml.text().toString();
            result->append(str);
        }

        if (xml.hasError() || xml.isEndDocument())
            break;
    }

    result->addType("Document");

    return;
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::OdfExtractor, "kfilemetadata_odfextractor")
