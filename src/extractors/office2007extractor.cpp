/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Vishesh Handa <me@vhanda.in>

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


#include "office2007extractor.h"

#include <KZip>

#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>

using namespace KFileMetaData;

Office2007Extractor::Office2007Extractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}


QStringList Office2007Extractor::mimetypes() const
{
    QStringList list;
    list << QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.document")
         << QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.presentation")
         << QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");

    return list;
}

void Office2007Extractor::extract(ExtractionResult* result)
{
    KZip zip(result->inputUrl());
    if (!zip.open(QIODevice::ReadOnly)) {
        qWarning() << "Document is not a valid ZIP archive";
        return;
    }

    const KArchiveDirectory* rootDir = zip.directory();
    if (!rootDir) {
        qWarning() << "Invalid document structure (main directory is missing)";
        return;
    }

    const QStringList rootEntries = rootDir->entries();
    if (!rootEntries.contains(QLatin1String("docProps"))) {
        qWarning() << "Invalid document structure (docProps is missing)";
        return;
    }

    const KArchiveEntry* docPropEntry = rootDir->entry(QLatin1String("docProps"));
    if (!docPropEntry->isDirectory()) {
        qWarning() << "Invalid document structure (docProps is not a directory)";
        return;
    }

    const KArchiveDirectory* docPropDirectory = dynamic_cast<const KArchiveDirectory*>(docPropEntry);
    const QStringList docPropsEntries = docPropDirectory->entries();

    if (docPropsEntries.contains(QLatin1String("core.xml"))) {
        QDomDocument coreDoc(QLatin1String("core"));
        const KArchiveFile* file = static_cast<const KArchiveFile*>(docPropDirectory->entry(QLatin1String("core.xml")));
        coreDoc.setContent(file->data());

        QDomElement docElem = coreDoc.documentElement();

        QDomElement elem = docElem.firstChildElement(QLatin1String("dc:description"));
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Comment, str);
            }
        }

        elem = docElem.firstChildElement(QLatin1String("dc:subject"));
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Subject, str);
            }
        }

        elem = docElem.firstChildElement(QLatin1String("dc:title"));
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Title, str);
            }
        }

        elem = docElem.firstChildElement(QLatin1String("dc:creator"));
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Author, str);
            }
        }

        elem = docElem.firstChildElement(QLatin1String("dc:language"));
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Langauge, str);
            }
        }

        elem = docElem.firstChildElement(QLatin1String("dcterms:created"));
        if (!elem.isNull()) {
            QString str = elem.text();
            QDateTime dt = dateTimeFromString(elem.text());
            if (!dt.isNull()) {
                result->add(Property::CreationDate, dt);
            }
        }

        elem = docElem.firstChildElement(QLatin1String("cp:keywords"));
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Keywords, str);
            }
        }
    }

    if (docPropsEntries.contains(QLatin1String("app.xml"))) {
        QDomDocument appDoc(QLatin1String("app"));
        const KArchiveFile* file = static_cast<const KArchiveFile*>(docPropDirectory->entry(QLatin1String("app.xml")));
        appDoc.setContent(file->data());

        QDomElement docElem = appDoc.documentElement();

        // According to the ontologies only Documents can have a wordCount and pageCount
        const QString mimeType = result->inputMimetype();
        if (mimeType == QLatin1String("application/vnd.openxmlformats-officedocument.wordprocessingml.document")) {
            QDomElement elem = docElem.firstChildElement(QLatin1String("Pages"));
            if (!elem.isNull()) {
                bool ok = false;
                int pageCount = elem.text().toInt(&ok);
                if (ok) {
                    result->add(Property::PageCount, pageCount);
                }
            }

            elem = docElem.firstChildElement(QLatin1String("Words"));
            if (!elem.isNull()) {
                bool ok = false;
                int wordCount = elem.text().toInt(&ok);
                if (ok) {
                    result->add(Property::WordCount, wordCount);
                }
            }
        }

        QDomElement elem = docElem.firstChildElement(QLatin1String("Application"));
        if (!elem.isNull()) {
            QString app = elem.text();
            if (!app.isEmpty()) {
                result->add(Property::Generator, app);
            }
        }
    }

    //
    // Plain Text
    //
    bool extractPlainText = (result->inputFlags() & ExtractionResult::ExtractPlainText);

    if (rootEntries.contains(QLatin1String("word"))) {
        result->addType(Type::Document);

        if (!extractPlainText)
            return;

        const KArchiveEntry* wordEntry = rootDir->entry(QLatin1String("word"));
        if (!wordEntry->isDirectory()) {
            qWarning() << "Invalid document structure (word is not a directory)";
            return;
        }

        const KArchiveDirectory* wordDirectory = dynamic_cast<const KArchiveDirectory*>(wordEntry);
        const QStringList wordEntries = wordDirectory->entries();

        if (wordEntries.contains(QLatin1String("document.xml"))) {
            QDomDocument appDoc(QLatin1String("document"));
            const KArchiveFile* file = static_cast<const KArchiveFile*>(wordDirectory->entry(QLatin1String("document.xml")));

            extractTextWithTag(file->createDevice(), QStringLiteral("w:t"), result);
        }
    }

    else if (rootEntries.contains(QLatin1String("xl"))) {
        result->addType(Type::Document);
        result->addType(Type::Spreadsheet);

        if (!extractPlainText)
            return;

        const KArchiveEntry* xlEntry = rootDir->entry(QLatin1String("xl"));
        if (!xlEntry->isDirectory()) {
            qWarning() << "Invalid document structure (xl is not a directory)";
            return;
        }

        const KArchiveDirectory* xlDirectory = dynamic_cast<const KArchiveDirectory*>(xlEntry);
        extractTextFromFiles(xlDirectory, result);
    }

    else if (rootEntries.contains(QLatin1String("ppt"))) {
        result->addType(Type::Document);
        result->addType(Type::Presentation);

        if (!extractPlainText)
            return;

        const KArchiveEntry* pptEntry = rootDir->entry(QLatin1String("ppt"));
        if (!pptEntry->isDirectory()) {
            qWarning() << "Invalid document structure (ppt is not a directory)";
            return;
        }

        const KArchiveDirectory* pptDirectory = dynamic_cast<const KArchiveDirectory*>(pptEntry);
        extractTextFromFiles(pptDirectory, result);
    }
}

void Office2007Extractor::extractAllText(QIODevice* device, ExtractionResult* result)
{
    QXmlStreamReader xml(device);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isCharacters()) {
            QString str = xml.text().toString();
            result->append(str);
        }

        if (xml.isEndDocument() || xml.hasError())
            break;
    }
}

void Office2007Extractor::extractTextFromFiles(const KArchiveDirectory* archiveDir, ExtractionResult* result)
{
    const QStringList entries = archiveDir->entries();
    foreach(const QString & entryName, entries) {
        const KArchiveEntry* entry = archiveDir->entry(entryName);
        if (entry->isDirectory()) {
            const KArchiveDirectory* subDir = dynamic_cast<const KArchiveDirectory*>(entry);
            extractTextFromFiles(subDir, result);
            continue;
        }

        if (!entryName.endsWith(QLatin1String(".xml")))
            continue;

        const KArchiveFile* file = static_cast<const KArchiveFile*>(entry);
        extractAllText(file->createDevice(), result);
    }
}

void Office2007Extractor::extractTextWithTag(QIODevice* device, const QString& tag, ExtractionResult* result)
{
    QXmlStreamReader xml(device);

    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.qualifiedName().startsWith(tag) && xml.isStartElement()) {
            QString str = xml.readElementText(QXmlStreamReader::IncludeChildElements);

            if (!str.isEmpty()) {
                result->append(str);
            }
        }

        if (xml.isEndDocument() || xml.hasError())
            break;
    }
}
