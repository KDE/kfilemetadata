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

#include <KDebug>
#include <KZip>

#include <QDomDocument>
#include <QXmlStreamReader>

using namespace KFileMetaData;

Office2007Extractor::Office2007Extractor(QObject* parent, const QVariantList&): ExtractorPlugin(parent)
{

}


QStringList Office2007Extractor::mimetypes() const
{
    QStringList list;
    list << QLatin1String("application/vnd.openxmlformats-officedocument.wordprocessingml.document")
         << QLatin1String("application/vnd.openxmlformats-officedocument.presentationml.presentation")
         << QLatin1String("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");

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
    if (!rootEntries.contains("docProps")) {
        qWarning() << "Invalid document structure (docProps is missing)";
        return;
    }

    const KArchiveEntry* docPropEntry = rootDir->entry("docProps");
    if (!docPropEntry->isDirectory()) {
        qWarning() << "Invalid document structure (docProps is not a directory)";
        return;
    }

    const KArchiveDirectory* docPropDirectory = dynamic_cast<const KArchiveDirectory*>(docPropEntry);
    const QStringList docPropsEntries = docPropDirectory->entries();

    if (docPropsEntries.contains("core.xml")) {
        QDomDocument coreDoc("core");
        const KArchiveFile* file = static_cast<const KArchiveFile*>(docPropDirectory->entry("core.xml"));
        coreDoc.setContent(file->data());

        QDomElement docElem = coreDoc.documentElement();

        QDomElement elem = docElem.firstChildElement("dc:description");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Description, str);
            }
        }

        elem = docElem.firstChildElement("dc:subject");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Subject, str);
            }
        }

        elem = docElem.firstChildElement("dc:title");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Title, str);
            }
        }

        elem = docElem.firstChildElement("dc:creator");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Creator, str);
            }
        }

        elem = docElem.firstChildElement("dc:langauge");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                result->add(Property::Langauge, str);
            }
        }
    }

    if (docPropsEntries.contains("app.xml")) {
        QDomDocument appDoc("app");
        const KArchiveFile* file = static_cast<const KArchiveFile*>(docPropDirectory->entry("app.xml"));
        appDoc.setContent(file->data());

        QDomElement docElem = appDoc.documentElement();

        // According to the ontologies only Documents can have a wordCount and pageCount
        const QString mimeType = result->inputMimetype();
        if (mimeType == QLatin1String("application/vnd.openxmlformats-officedocument.wordprocessingml.document")) {
            QDomElement elem = docElem.firstChildElement("Pages");
            if (!elem.isNull()) {
                bool ok = false;
                int pageCount = elem.text().toInt(&ok);
                if (ok) {
                    result->add(Property::PageCount, pageCount);
                }
            }

            elem = docElem.firstChildElement("Words");
            if (!elem.isNull()) {
                bool ok = false;
                int wordCount = elem.text().toInt(&ok);
                if (ok) {
                    result->add(Property::WordCount, wordCount);
                }
            }
        }

        QDomElement elem = docElem.firstChildElement("Application");
        if (!elem.isNull()) {
            QString app = elem.text();
            if (!app.isEmpty()) {
                result->add(Property::Generator, app);
            }
        }
    }


    if (rootEntries.contains("word")) {
        const KArchiveEntry* wordEntry = rootDir->entry("word");
        if (!wordEntry->isDirectory()) {
            qWarning() << "Invalid document structure (word is not a directory)";
            return;
        }

        const KArchiveDirectory* wordDirectory = dynamic_cast<const KArchiveDirectory*>(wordEntry);
        const QStringList wordEntries = wordDirectory->entries();

        if (wordEntries.contains("document.xml")) {
            QDomDocument appDoc("document");
            const KArchiveFile* file = static_cast<const KArchiveFile*>(wordDirectory->entry("document.xml"));

            extractTextWithTag(file->createDevice(), QLatin1String("w:t"), result);
        }

        result->addType(Type::Document);
    }

    else if (rootEntries.contains("xl")) {
        const KArchiveEntry* xlEntry = rootDir->entry("xl");
        if (!xlEntry->isDirectory()) {
            qWarning() << "Invalid document structure (xl is not a directory)";
            return;
        }

        const KArchiveDirectory* xlDirectory = dynamic_cast<const KArchiveDirectory*>(xlEntry);
        extractTextFromFiles(xlDirectory, result);

        result->addType(Type::Document);
        result->addType(Type::Spreadsheet);
    }

    else if (rootEntries.contains("ppt")) {
        const KArchiveEntry* pptEntry = rootDir->entry("ppt");
        if (!pptEntry->isDirectory()) {
            qWarning() << "Invalid document structure (ppt is not a directory)";
            return;
        }

        const KArchiveDirectory* pptDirectory = dynamic_cast<const KArchiveDirectory*>(pptEntry);
        extractTextFromFiles(pptDirectory, result);

        result->addType(Type::Document);
        result->addType(Type::Presentation);
    }

    return;
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

        if (!entryName.endsWith(".xml"))
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

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::Office2007Extractor, "kfilemetadata_office2007extractor")
