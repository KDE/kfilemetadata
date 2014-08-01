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

#include <KZip>
#include <KService>

#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>

using namespace KFileMetaData;

OdfExtractor::OdfExtractor(QObject* parent, const QVariantList&): ExtractorPlugin(parent)
{

}

QStringList OdfExtractor::mimetypes() const
{
    QStringList list;
    list << QStringLiteral("application/vnd.oasis.opendocument.text")
         << QStringLiteral("application/vnd.oasis.opendocument.presentation")
         << QStringLiteral("application/vnd.oasis.opendocument.spreadsheet");

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
            if (tagName == QStringLiteral("dc:description")) {
                result->add(Property::Comment, e.text());
            } else if (tagName == QStringLiteral("dc:subject")) {
                result->add(Property::Subject, e.text());
            } else if (tagName == QStringLiteral("dc:title")) {
                result->add(Property::Title, e.text());
            } else if (tagName == QStringLiteral("dc:creator")) {
                result->add(Property::Author, e.text());
            } else if (tagName == QStringLiteral("dc:langauge")) {
                result->add(Property::Langauge, e.text());
            }

            // Meta Properties
            else if (tagName == QStringLiteral("meta:document-statistic")) {
                bool ok = false;
                int pageCount = e.attribute("meta:page-count").toInt(&ok);
                if (ok) {
                    result->add(Property::PageCount, pageCount);
                }

                int wordCount = e.attribute("meta:word-count").toInt(&ok);
                if (ok) {
                    result->add(Property::WordCount, wordCount);
                }
            } else if (tagName == QStringLiteral("meta:keyword")) {
                QString keywords = e.text();
                    result->add(Property::Keywords, keywords);
            } else if (tagName == QStringLiteral("meta:generator")) {
                result->add(Property::Generator, e.text());
            } else if (tagName == QStringLiteral("meta:creation-date")) {
                QDateTime dt = ExtractorPlugin::dateTimeFromString(e.text());
                if (!dt.isNull())
                    result->add(Property::CreationDate, dt);
            }
        }
        n = n.nextSibling();
    }

    result->addType(Type::Document);
    if (result->inputMimetype() == QStringLiteral("application/vnd.oasis.opendocument.presentation")) {
        result->addType(Type::Presentation);
    }
    else if (result->inputMimetype() == QStringLiteral("application/vnd.oasis.opendocument.spreadsheet")) {
        result->addType(Type::Spreadsheet);
    }

    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
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
}

K_PLUGIN_FACTORY(factory, registerPlugin<OdfExtractor>();)

#include "odfextractor.moc"
