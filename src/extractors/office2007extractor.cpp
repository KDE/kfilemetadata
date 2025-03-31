/*
    SPDX-FileCopyrightText: 2013 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "kfilemetadata_debug.h"
#include "office2007extractor.h"

#include "dublincoreextractor.h"
#include <memory>

#include <KZip>

#include <QDomDocument>
#include <QXmlStreamReader>

using namespace KFileMetaData;

namespace {
inline QString cpNS()       { return QStringLiteral("http://schemas.openxmlformats.org/package/2006/metadata/core-properties"); }
inline QString relNS()      { return QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships"); }
inline QString extPropNST() { return QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"); }
inline QString extPropNSS() { return QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/extendedProperties"); }

inline QString coreProp()   { return QStringLiteral("http://schemas.openxmlformats.org/package/2006/relationships/metadata/core-properties"); }
inline QString extPropT()   { return QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships/extended-properties"); }
inline QString extPropS()   { return QStringLiteral("http://purl.oclc.org/ooxml/officeDocument/relationships/extendedProperties"); }
} // namespace

Office2007Extractor::Office2007Extractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}

const QStringList supportedMimeTypes = {
    QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.document"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.wordprocessingml.template"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.presentation"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.slide"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.slideshow"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.presentationml.template"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"),
    QStringLiteral("application/vnd.openxmlformats-officedocument.spreadsheetml.template"),
    QStringLiteral("application/vnd.ms-xpsdocument"),
    QStringLiteral("application/oxps"),
    QStringLiteral("model/3mf"),
};

QStringList Office2007Extractor::mimetypes() const
{
    return supportedMimeTypes;
}

void Office2007Extractor::extract(ExtractionResult* result)
{
    KZip zip(result->inputUrl());
    if (!zip.open(QIODevice::ReadOnly)) {
        qCWarning(KFILEMETADATA_LOG) << "Failed to open" << zip.fileName() << "-" << zip.errorString();
        return;
    }

    const KArchiveDirectory* rootDir = zip.directory();
    if (!rootDir) {
        qCWarning(KFILEMETADATA_LOG) << "Invalid document structure (main directory is missing)";
        return;
    }

    const bool extractMetaData = result->inputFlags() & ExtractionResult::ExtractMetaData;

    // Resolve part relationships according to ECMA-376-2 (Open Packaging Conventions, OPC)
    const QDomElement relationsElem = [rootDir]() {
        const KArchiveFile *baseRels = rootDir->file(QStringLiteral("_rels/.rels"));
        if (!baseRels) {
            qCWarning(KFILEMETADATA_LOG) << "Invalid document structure - missing package relationship";
            return QDomElement{};
        }

        QDomDocument relationsDoc;
        relationsDoc.setContent(baseRels->data(), QDomDocument::ParseOption::UseNamespaceProcessing);

        auto relations = relationsDoc.firstChildElement(QStringLiteral("Relationships"));
        if (relations.isNull()) {
            qCWarning(KFILEMETADATA_LOG) << "Invalid document structure - invalid package relationships";
        }
        return relations;
    }();

    auto targetByType = [&relationsElem](const QString &type, const QString &defVal = {}) -> QString {
        for (auto rel = relationsElem.firstChildElement(); !rel.isNull(); rel = rel.nextSiblingElement()) {
            if (rel.namespaceURI() == relNS() && rel.localName() == QStringLiteral("Relationship")
                && rel.attribute(QStringLiteral("Type")) == type) {
                return rel.attribute(QStringLiteral("Target"));
            }
        }
        return defVal;
    };

    // Core Properties
    const QString corePropertiesFile = targetByType(coreProp(), QStringLiteral("docProps/core.xml"));
    if (const KArchiveFile *file = extractMetaData ? rootDir->file(corePropertiesFile) : nullptr; file) {
        QDomDocument coreDoc(QStringLiteral("core"));
        coreDoc.setContent(file->data(), QDomDocument::ParseOption::UseNamespaceProcessing);

        QDomElement cpElem = coreDoc.documentElement();

        if (!cpElem.isNull() && cpElem.namespaceURI() == cpNS()) {
            DublinCoreExtractor::extract(result, cpElem);
        }

        auto elem = cpElem.firstChildElement(QStringLiteral("keywords"));
        if (!elem.isNull() && elem.namespaceURI() == cpNS()) {
            for (auto c = elem.firstChild(); !c.isNull(); c = c.nextSibling()) {
                if (const auto childElem = c.toElement(); childElem.localName() == QStringLiteral("value") && !childElem.text().isEmpty()) {
                    result->add(Property::Keywords, childElem.text());
                } else if (const auto tNode = c.toText(); !tNode.nodeValue().isEmpty()) {
                    result->add(Property::Keywords, tNode.nodeValue());
                }
            }
        }
    }

    // Extended Properties - two valid relation types: "strict" (ECMA-376-1:2016) or "transitional" (ECMA-367-4:2016)
    const QString extPropertiesFile = targetByType(extPropS(), targetByType(extPropT(), QStringLiteral("docProps/app.xml")));
    if (const KArchiveFile *file = extractMetaData ? rootDir->file(extPropertiesFile) : nullptr; file) {
        QDomDocument appDoc;
        appDoc.setContent(file->data(), QDomDocument::ParseOption::UseNamespaceProcessing);

        QDomElement propsElem = appDoc.documentElement();

        for (auto prop = propsElem.firstChildElement(); !prop.isNull(); prop = prop.nextSiblingElement()) {
            // Look for properties as specified in ECMA-376-1, Annex A.6.2 Extended Properties
            bool ok;
            if (prop.localName() == QStringLiteral("Pages")) {
                if (int count = prop.text().toInt(&ok); ok == true) {
                    result->add(Property::PageCount, count);
                }
            } else if (prop.localName() == QStringLiteral("Slides")) {
                if (int count = prop.text().toInt(&ok); ok == true) {
                    // Map number of slides to PageCount
                    result->add(Property::PageCount, count);
                }
            } else if (prop.localName() == QStringLiteral("Words")) {
                if (int count = prop.text().toInt(&ok); ok == true) {
                    result->add(Property::WordCount, count);
                }
            } else if (prop.localName() == QStringLiteral("Lines")) {
                if (int count = prop.text().toInt(&ok); ok == true) {
                    result->add(Property::LineCount, count);
                }
            } else if (prop.localName() == QStringLiteral("Application")) {
                QString application = prop.text();
                if (!application.isEmpty()) {
                    result->add(Property::Generator, application);
                }
            }
        }
    }

    //
    // Plain Text
    //
    bool extractPlainText = (result->inputFlags() & ExtractionResult::ExtractPlainText);

    if (const auto wordEntry = rootDir->entry(QStringLiteral("word")); wordEntry) {
        result->addType(Type::Document);

        if (!extractPlainText) {
            return;
        }

        if (!wordEntry->isDirectory()) {
            qCWarning(KFILEMETADATA_LOG) << "Invalid document structure (word is not a directory)";
            return;
        }

        const KArchiveDirectory* wordDirectory = dynamic_cast<const KArchiveDirectory*>(wordEntry);
        const QStringList wordEntries = wordDirectory->entries();

        if (wordEntries.contains(QStringLiteral("document.xml"))) {
            const KArchiveFile* file = wordDirectory->file(QStringLiteral("document.xml"));

            if (file) {
                std::unique_ptr<QIODevice> contentIODevice{file->createDevice()};
                extractTextWithTag(contentIODevice.get(), QStringLiteral("w:t"), result);
            }
        }
    }

    else if (const auto xlEntry = rootDir->entry(QStringLiteral("xl")); xlEntry) {
        result->addType(Type::Document);
        result->addType(Type::Spreadsheet);

        if (!extractPlainText) {
            return;
        }

        if (!xlEntry->isDirectory()) {
            qCWarning(KFILEMETADATA_LOG) << "Invalid document structure (xl is not a directory)";
            return;
        }

        const auto xlDirectory = dynamic_cast<const KArchiveDirectory*>(xlEntry);
        // TODO: Read the sheets from worksheets/*.xml, and dereference all cells
        // values in order
        const KArchiveFile* file = xlDirectory->file(QStringLiteral("sharedStrings.xml"));
        if (!file) {
            return;
        }
        std::unique_ptr<QIODevice> contentIODevice{file->createDevice()};
        extractTextWithTag(contentIODevice.get(), QStringLiteral("t"), result);
    }

    else if (const auto pptEntry = rootDir->entry(QStringLiteral("ppt")); pptEntry) {
        result->addType(Type::Document);
        result->addType(Type::Presentation);

        if (!extractPlainText) {
            return;
        }

        if (!pptEntry->isDirectory()) {
            qCWarning(KFILEMETADATA_LOG) << "Invalid document structure (ppt is not a directory)";
            return;
        }

        const auto pptDirectory = dynamic_cast<const KArchiveDirectory*>(pptEntry);
        const auto slidesEntry = pptDirectory->entry(QStringLiteral("slides"));
        if (!slidesEntry || !slidesEntry->isDirectory()) {
            return;
        }

        const auto slidesDirectory = dynamic_cast<const KArchiveDirectory*>(slidesEntry);
        QStringList entries = slidesDirectory->entries();
        // TODO: Read the actual order from presentation.xml, and follow the
        // references in ppt/_rels/presentation.xml.rel
        std::sort(entries.begin(), entries.end());
        for (const QString & entryName : std::as_const(entries)) {
            const KArchiveFile* file = slidesDirectory->file(entryName);
            if (!file) {
                continue;
            }
            std::unique_ptr<QIODevice> contentIODevice{file->createDevice()};
            extractTextWithTag(contentIODevice.get(), QStringLiteral("a:t"), result);
        }
    }

    else if (!relationsElem.isNull()) {
        // Any other document type likely following OPC
        result->addType(Type::Document);
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

        if (xml.isEndDocument() || xml.hasError()) {
            break;
        }
    }
}

#include "moc_office2007extractor.cpp"
