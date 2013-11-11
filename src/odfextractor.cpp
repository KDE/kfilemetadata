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

#include "nco.h"
#include "nie.h"
#include "nfo.h"

#include <KDE/KDebug>
#include <KDE/KZip>

#include <QtXml/QDomDocument>
#include <QtXml/QXmlStreamReader>
#include <Soprano/Vocabulary/NAO>

using namespace Soprano::Vocabulary;
using namespace Nepomuk2::Vocabulary;
using namespace Nepomuk2;

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

SimpleResourceGraph OdfExtractor::extract(const QUrl& resUri, const QUrl& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    KZip zip(fileUrl.toLocalFile());
    if (!zip.open(QIODevice::ReadOnly)) {
        qWarning() << "Document is not a valid ZIP archive";
        return SimpleResourceGraph();
    }

    const KArchiveDirectory* directory = zip.directory();
    if (!directory) {
        qWarning() << "Invalid document structure (main directory is missing)";
        return SimpleResourceGraph();
    }

    const QStringList entries = directory->entries();
    if (!entries.contains("meta.xml")) {
        qWarning() << "Invalid document structure (meta.xml is missing)";
        return SimpleResourceGraph();
    }

    SimpleResourceGraph graph;
    SimpleResource fileRes(resUri);

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
                fileRes.addProperty(NAO::description(), e.text());
            } else if (tagName == QLatin1String("dc:subject")) {
                fileRes.addProperty(NIE::subject(), e.text());
            } else if (tagName == QLatin1String("dc:title")) {
                fileRes.setProperty(NIE::title(), e.text());
            } else if (tagName == QLatin1String("dc:creator")) {
                SimpleResource creator;
                creator.addType(NCO::Contact());
                creator.addProperty(NCO::fullname(), e.text());
                graph << creator;

                fileRes.setProperty(NCO::creator(), creator);
            } else if (tagName == QLatin1String("dc:langauge")) {
                fileRes.setProperty(NIE::language(), e.text());
            }

            // Meta Properties
            else if (tagName == QLatin1String("meta:document-statistic")) {
                bool ok = false;
                int pageCount = e.attribute("meta:page-count").toInt(&ok);
                if (ok) {
                    fileRes.setProperty(NFO::pageCount(), pageCount);
                }

                int wordCount = e.attribute("meta:word-count").toInt(&ok);
                if (ok) {
                    fileRes.setProperty(NFO::wordCount(), wordCount);
                }
            } else if (tagName == QLatin1String("meta:keyword")) {
                QString keywords = e.text();
                fileRes.addProperty(NIE::keyword(), keywords);
            } else if (tagName == QLatin1String("meta:generator")) {
                fileRes.addProperty(NIE::generator(), e.text());
            } else if (tagName == QLatin1String("meta:creation-date")) {
                QDateTime dt = ExtractorPlugin::dateTimeFromString(e.text());
                if (!dt.isNull())
                    fileRes.addProperty(NIE::contentCreated(), dt);
            }
        }
        n = n.nextSibling();
    }

    const KArchiveFile* contentsFile = static_cast<const KArchiveFile*>(directory->entry("content.xml"));
    QXmlStreamReader xml(contentsFile->createDevice());

    QString plainText;
    QTextStream stream(&plainText);

    int size = 0;
    while (!xml.atEnd()) {
        if (size >= maxPlainTextSize())
            break;

        xml.readNext();
        if (xml.isCharacters()) {
            QString str = xml.text().toString();
            stream << str;
            size += str.size();

            if (!str.at(str.length() - 1).isSpace())
                stream << QLatin1Char(' ');
        }

        if (xml.hasError() || xml.isEndDocument())
            break;
    }

    if (!plainText.isEmpty())
        fileRes.addProperty(NIE::plainTextContent(), plainText);

    if (fileRes.properties().isEmpty())
        return SimpleResourceGraph();

    graph << fileRes;
    return graph;
}


NEPOMUK_EXPORT_EXTRACTOR(Nepomuk2::OdfExtractor, "nepomukodfextractor")
