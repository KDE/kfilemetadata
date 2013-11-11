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

Office2007Extractor::Office2007Extractor(QObject* parent, const QVariantList&): ExtractorPlugin(parent)
{

}


QStringList Office2007Extractor::mimetypes()
{
    QStringList list;
    list << QLatin1String("application/vnd.openxmlformats-officedocument.wordprocessingml.document")
         << QLatin1String("application/vnd.openxmlformats-officedocument.presentationml.presentation")
         << QLatin1String("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet");

    return list;
}

SimpleResourceGraph Office2007Extractor::extract(const QUrl& resUri, const QUrl& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    KZip zip(fileUrl.toLocalFile());
    if (!zip.open(QIODevice::ReadOnly)) {
        qWarning() << "Document is not a valid ZIP archive";
        return SimpleResourceGraph();
    }

    const KArchiveDirectory* rootDir = zip.directory();
    if (!rootDir) {
        qWarning() << "Invalid document structure (main directory is missing)";
        return SimpleResourceGraph();
    }

    const QStringList rootEntries = rootDir->entries();
    if (!rootEntries.contains("docProps")) {
        qWarning() << "Invalid document structure (docProps is missing)";
        return SimpleResourceGraph();
    }

    const KArchiveEntry* docPropEntry = rootDir->entry("docProps");
    if (!docPropEntry->isDirectory()) {
        qWarning() << "Invalid document structure (docProps is not a directory)";
        return SimpleResourceGraph();
    }

    SimpleResourceGraph graph;
    SimpleResource fileRes(resUri);

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
                fileRes.setProperty(NAO::description(), str);
            }
        }

        elem = docElem.firstChildElement("dc:subject");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                fileRes.setProperty(NIE::subject(), str);
            }
        }

        elem = docElem.firstChildElement("dc:title");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                fileRes.setProperty(NIE::title(), str);
            }
        }

        elem = docElem.firstChildElement("dc:creator");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                SimpleResource creator;
                creator.addType(NCO::Contact());
                creator.addProperty(NCO::fullname(), str);
                graph << creator;

                fileRes.setProperty(NCO::creator(), creator);
            }
        }

        elem = docElem.firstChildElement("dc:langauge");
        if (!elem.isNull()) {
            QString str = elem.text();
            if (!str.isEmpty()) {
                fileRes.setProperty(NIE::language(), str);
            }
        }
    }

    if (docPropsEntries.contains("app.xml")) {
        QDomDocument appDoc("app");
        const KArchiveFile* file = static_cast<const KArchiveFile*>(docPropDirectory->entry("app.xml"));
        appDoc.setContent(file->data());

        QDomElement docElem = appDoc.documentElement();

        // According to the ontologies only Documents can have a wordCount and pageCount
        if (mimeType == QLatin1String("application/vnd.openxmlformats-officedocument.wordprocessingml.document")) {
            QDomElement elem = docElem.firstChildElement("Pages");
            if (!elem.isNull()) {
                bool ok = false;
                int pageCount = elem.text().toInt(&ok);
                if (ok) {
                    fileRes.setProperty(NFO::pageCount(), pageCount);
                }
            }

            elem = docElem.firstChildElement("Words");
            if (!elem.isNull()) {
                bool ok = false;
                int wordCount = elem.text().toInt(&ok);
                if (ok) {
                    fileRes.setProperty(NFO::wordCount(), wordCount);
                }
            }
        }

        QDomElement elem = docElem.firstChildElement("Application");
        if (!elem.isNull()) {
            QString app = elem.text();
            if (!app.isEmpty()) {
                fileRes.setProperty(NIE::generator(), app);
            }
        }
    }


    if (rootEntries.contains("word")) {
        const KArchiveEntry* wordEntry = rootDir->entry("word");
        if (!wordEntry->isDirectory()) {
            qWarning() << "Invalid document structure (word is not a directory)";
            return SimpleResourceGraph();
        }

        const KArchiveDirectory* wordDirectory = dynamic_cast<const KArchiveDirectory*>(wordEntry);
        const QStringList wordEntries = wordDirectory->entries();

        if (wordEntries.contains("document.xml")) {
            QDomDocument appDoc("document");
            const KArchiveFile* file = static_cast<const KArchiveFile*>(wordDirectory->entry("document.xml"));

            QString plainText;
            QTextStream stream(&plainText);

            extractTextWithTag(file->createDevice(), QLatin1String("w:t"), stream);
            if (!plainText.isEmpty())
                fileRes.addProperty(NIE::plainTextContent(), plainText);
        }
    }

    else if (rootEntries.contains("xl")) {
        const KArchiveEntry* xlEntry = rootDir->entry("xl");
        if (!xlEntry->isDirectory()) {
            qWarning() << "Invalid document structure (xl is not a directory)";
            return SimpleResourceGraph();
        }

        QString plainText;
        QTextStream stream(&plainText);

        const KArchiveDirectory* xlDirectory = dynamic_cast<const KArchiveDirectory*>(xlEntry);
        extractTextFromFiles(xlDirectory, stream);
        if (!plainText.isEmpty())
            fileRes.addProperty(NIE::plainTextContent(), plainText);
    }

    else if (rootEntries.contains("ppt")) {
        const KArchiveEntry* pptEntry = rootDir->entry("ppt");
        if (!pptEntry->isDirectory()) {
            qWarning() << "Invalid document structure (ppt is not a directory)";
            return SimpleResourceGraph();
        }

        QString plainText;
        QTextStream stream(&plainText);

        const KArchiveDirectory* pptDirectory = dynamic_cast<const KArchiveDirectory*>(pptEntry);
        extractTextFromFiles(pptDirectory, stream);
        if (!plainText.isEmpty())
            fileRes.addProperty(NIE::plainTextContent(), plainText);
    }

    if (fileRes.properties().isEmpty())
        return SimpleResourceGraph();

    graph << fileRes;
    return graph;
}

void Office2007Extractor::extractAllText(QIODevice* device, QTextStream& stream)
{
    QXmlStreamReader xml(device);

    while (!xml.atEnd()) {
        if (stream.string()->size() >= maxPlainTextSize())
            return;

        xml.readNext();
        if (xml.isCharacters()) {
            QString str = xml.text().toString();
            stream << str;

            if (!str.at(str.length() - 1).isSpace())
                stream << QLatin1Char(' ');
        }

        if (xml.isEndDocument() || xml.hasError())
            break;
    }
}

void Office2007Extractor::extractTextFromFiles(const KArchiveDirectory* archiveDir, QTextStream& stream)
{
    const QStringList entries = archiveDir->entries();
    foreach(const QString & entryName, entries) {
        const KArchiveEntry* entry = archiveDir->entry(entryName);
        if (entry->isDirectory()) {
            const KArchiveDirectory* subDir = dynamic_cast<const KArchiveDirectory*>(entry);
            extractTextFromFiles(subDir, stream);
            continue;
        }

        if (stream.string()->size() >= maxPlainTextSize())
            return;

        if (!entryName.endsWith(".xml"))
            continue;

        const KArchiveFile* file = static_cast<const KArchiveFile*>(entry);
        extractAllText(file->createDevice(), stream);
    }
}

void Office2007Extractor::extractTextWithTag(QIODevice* device, const QString& tag, QTextStream& stream)
{
    QXmlStreamReader xml(device);
    int size = 0;

    while (!xml.atEnd()) {
        if (size >= maxPlainTextSize())
            break;

        xml.readNext();
        if (xml.qualifiedName().startsWith(tag) && xml.isStartElement()) {
            QString str = xml.readElementText(QXmlStreamReader::IncludeChildElements).simplified();

            if (!str.isEmpty()) {
                stream << str;
                size += str.size();

                if (!str.at(str.length() - 1).isSpace())
                    stream << QLatin1Char(' ');
            }
        }

        if (xml.isEndDocument() || xml.hasError())
            break;
    }
}


NEPOMUK_EXPORT_EXTRACTOR(Nepomuk2::Office2007Extractor, "nepomukoffice2007extractor")
