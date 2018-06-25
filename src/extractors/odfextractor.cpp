/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Vishesh Handa <me@vhanda.in>
    Copyright (C) 2012  Jörg Ehrichs <joerg.ehrichs@gmx.de>
    Copyright (C) 2016  Christoph Cullmann <cullmann@kde.org>

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

#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>

namespace {

inline QString dcNS()     { return QStringLiteral("http://purl.org/dc/elements/1.1/"); }
inline QString metaNS()   { return QStringLiteral("urn:oasis:names:tc:opendocument:xmlns:meta:1.0"); }
inline QString officeNS() { return QStringLiteral("urn:oasis:names:tc:opendocument:xmlns:office:1.0"); }

QDomElement firstChildElementNS(const QDomNode &node, const QString &nsURI, const QString &localName)
{
    for (auto e = node.firstChildElement(); !e.isNull(); e = e.nextSiblingElement()) {
        if (e.localName() == localName && e.namespaceURI() == nsURI) {
            return e;
        }
    }

    return QDomElement();
}

}

using namespace KFileMetaData;

OdfExtractor::OdfExtractor(QObject* parent)
    : ExtractorPlugin(parent)
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

    // we need a meta xml file in the archive!
    const auto metaXml = directory->entry(QStringLiteral("meta.xml"));
    if (!metaXml || !metaXml->isFile()) {
        qWarning() << "Invalid document structure (meta.xml is missing)";
        return;
    }

    QDomDocument metaData(QStringLiteral("metaData"));
    metaData.setContent(static_cast<const KArchiveFile*>(metaXml)->data(), true);

    // parse metadata ...
    QDomElement meta = firstChildElementNS(firstChildElementNS(metaData,
                                                               officeNS(), QStringLiteral("document-meta")),
                                                               officeNS(), QStringLiteral("meta"));

    QDomNode n = meta.firstChild();
    while (!n.isNull()) {
        QDomElement e = n.toElement();
        if (!e.isNull()) {
            const QString namespaceURI = e.namespaceURI();
            const QString localName = e.localName();

            // Dublin Core
            if (namespaceURI == dcNS()) {
                if (localName == QLatin1String("description")) {
                    result->add(Property::Comment, e.text());
                } else if (localName == QLatin1String("subject")) {
                    result->add(Property::Subject, e.text());
                } else if (localName == QLatin1String("title")) {
                    result->add(Property::Title, e.text());
                } else if (localName == QLatin1String("creator")) {
                    result->add(Property::Author, e.text());
                } else if (localName == QLatin1String("language")) {
                    result->add(Property::Langauge, e.text());
                }
            }
            // Meta Properties
            else if (namespaceURI == metaNS()) {
                if (localName == QLatin1String("document-statistic")) {
                    bool ok = false;
                    int pageCount = e.attributeNS(metaNS(), QStringLiteral("page-count")).toInt(&ok);
                    if (ok) {
                        result->add(Property::PageCount, pageCount);
                    }

                    int wordCount = e.attributeNS(metaNS(), QStringLiteral("word-count")).toInt(&ok);
                    if (ok) {
                        result->add(Property::WordCount, wordCount);
                    }
                } else if (localName == QLatin1String("keyword")) {
                    QString keywords = e.text();
                    result->add(Property::Keywords, keywords);
                } else if (localName == QLatin1String("generator")) {
                    result->add(Property::Generator, e.text());
                } else if (localName == QLatin1String("creation-date")) {
                    QDateTime dt = ExtractorPlugin::dateTimeFromString(e.text());
                    if (!dt.isNull())
                        result->add(Property::CreationDate, dt);
                }
            }
        }
        n = n.nextSibling();
    }

    result->addType(Type::Document);
    if (result->inputMimetype() == QLatin1String("application/vnd.oasis.opendocument.presentation")) {
        result->addType(Type::Presentation);
    }
    else if (result->inputMimetype() == QLatin1String("application/vnd.oasis.opendocument.spreadsheet")) {
        result->addType(Type::Spreadsheet);
    }

    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    // for content indexing, we need content xml file
    const auto contentXml = directory->entry(QStringLiteral("content.xml"));
    if (!contentXml || !contentXml->isFile()) {
        qWarning() << "Invalid document structure (content.xml is missing)";
        return;
    }

    QXmlStreamReader xml(static_cast<const KArchiveFile*>(contentXml)->createDevice());
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
