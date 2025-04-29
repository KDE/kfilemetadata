// SPDX-FileCopyrightText: 2018 Martin T. H. Sandsmark <martin.sandsmark@kde.org>
// SPDX-FileCopyrightText: 2021 Carl Schwan <carl@carlschwan.eu>
// SPDX-License-Identifier: BSD-3-Clause

#include "epubcontainer.h"

#include <KArchiveDirectory>
#include <KArchiveFile>

#include <QDebug>
#include <QDir>
#include <QDomDocument>
#include <QImage>
#include <QImageReader>
#include <QScopedPointer>

#include "kfilemetadata_debug.h"

using namespace Qt::StringLiterals;

static constexpr const auto metadataFolderName = "META-INF"_L1;
static constexpr const auto mimetypeFileName = "mimetype"_L1;
static constexpr const auto containerFileName = "META-INF/container.xml"_L1;

bool EPubContainer::openFile(const QString &path)
{
    m_archive = std::make_unique<KZip>(path);

    if (!m_archive->open(QIODevice::ReadOnly)) {
        m_errorString = u"Failed to open %1"_s.arg(path);
        return false;
    }

    m_rootFolder = m_archive->directory();
    if (!m_rootFolder) {
        m_errorString = u"Failed to read %1"_s.arg(path);
        return false;
    }

    if (!parseMimetype()) {
        return false;
    }

    if (!parseContainer()) {
        return false;
    }

    return true;
}

QSharedPointer<QIODevice> EPubContainer::ioDevice(const QString &path)
{
    const KArchiveFile *archive = file(path);
    if (!archive) {
        m_errorString = u"Unable to open file %1"_s.arg(path);
        return QSharedPointer<QIODevice>();
    }

    return QSharedPointer<QIODevice>(archive->createDevice());
}

QImage EPubContainer::image(const QString &id)
{
    if (!m_items.contains(id)) {
        qCWarning(KFILEMETADATA_LOG) << "Asked for unknown item" << id << m_items.keys();
        return {};
    }

    const EpubItem &item = m_items.value(id);

    if (!QImageReader::supportedMimeTypes().contains(item.mimetype)) {
        qCWarning(KFILEMETADATA_LOG) << "Asked for unsupported type" << item.mimetype;
        return {};
    }

    QSharedPointer<QIODevice> device = ioDevice(item.path);

    if (!device) {
        return {};
    }

    return QImage::fromData(device->readAll());
}

QStringList EPubContainer::metadata(const QStringView &key) const
{
    return m_metadata.value(key);
}

bool EPubContainer::parseMimetype()
{
    Q_ASSERT(m_rootFolder);

    const KArchiveFile *mimetypeFile = m_rootFolder->file(mimetypeFileName);

    if (!mimetypeFile) {
        m_errorString = u"Unable to find mimetype in file"_s;
        return false;
    }

    QScopedPointer<QIODevice> ioDevice(mimetypeFile->createDevice());
    QByteArray mimetype = ioDevice->readAll();
    if (mimetype != "application/epub+zip") {
        qCWarning(KFILEMETADATA_LOG) << "Unexpected mimetype" << mimetype;
    }

    return true;
}

bool EPubContainer::parseContainer()
{
    Q_ASSERT(m_rootFolder);

    const KArchiveFile *containerFile = file(containerFileName);
    if (!containerFile) {
        m_errorString = u"Unable to find container information"_s;
        return false;
    }

    QScopedPointer<QIODevice> ioDevice(containerFile->createDevice());
    Q_ASSERT(ioDevice);

    // The only thing we need from this file is the path to the root file
    QDomDocument document;
    document.setContent(ioDevice.data());
    QDomNodeList rootNodes = document.elementsByTagName(u"rootfile"_s);
    for (int i = 0; i < rootNodes.count(); i++) {
        QDomElement rootElement = rootNodes.at(i).toElement();
        QString rootfilePath = rootElement.attribute(u"full-path"_s);
        if (rootfilePath.isEmpty()) {
            qCWarning(KFILEMETADATA_LOG) << "Invalid root file entry";
            continue;
        }
        if (parseContentFile(rootfilePath)) {
            return true;
        }
    }

    // Limitations:
    //  - We only read one rootfile
    //  - We don't read the following from META-INF/
    //     - manifest.xml (unknown contents, just reserved)
    //     - metadata.xml (unused according to spec, just reserved)
    //     - rights.xml (reserved for DRM, not standardized)
    //     - signatures.xml (signatures for files, standardized)

    m_errorString = u"Unable to find and use any content files"_s;
    return false;
}

bool EPubContainer::parseContentFile(const QString &filepath)
{
    const KArchiveFile *rootFile = file(filepath);
    if (!rootFile) {
        m_errorString = u"Malformed metadata, unable to get content metadata path"_s;
        return false;
    }
    QScopedPointer<QIODevice> ioDevice(rootFile->createDevice());
    QDomDocument document;
    document.setContent(ioDevice.data(), QDomDocument::ParseOption::UseNamespaceProcessing); // turn on namespace processing

    QDomNodeList metadataNodeList = document.elementsByTagName(u"metadata"_s);
    for (int i = 0; i < metadataNodeList.count(); i++) {
        QDomNodeList metadataChildList = metadataNodeList.at(i).childNodes();
        for (int j = 0; j < metadataChildList.count(); j++) {
            parseMetadataItem(metadataChildList.at(j), metadataChildList);
        }
    }

    // Extract current path, for resolving relative paths
    QString contentFileFolder;
    int separatorIndex = filepath.lastIndexOf(QLatin1Char('/'));
    if (separatorIndex > 0) {
        contentFileFolder = filepath.left(separatorIndex + 1);
    }

    // Parse out all the components/items in the epub
    QDomNodeList manifestNodeList = document.elementsByTagName(u"manifest"_s);
    for (int i = 0; i < manifestNodeList.count(); i++) {
        QDomElement manifestElement = manifestNodeList.at(i).toElement();
        QDomNodeList manifestItemList = manifestElement.elementsByTagName(u"item"_s);

        for (int j = 0; j < manifestItemList.count(); j++) {
            parseManifestItem(manifestItemList.at(j), contentFileFolder);
        }
    }

    // Parse out the document order
    QDomNodeList spineNodeList = document.elementsByTagName("spine"_L1);
    for (int i = 0; i < spineNodeList.count(); i++) {
        QDomElement spineElement = spineNodeList.at(i).toElement();

        QString tocId = spineElement.attribute("toc"_L1);
        if (!tocId.isEmpty() && m_items.contains(tocId)) {
            EpubPageReference tocReference;
            tocReference.title = u"Table of Contents"_s;
            tocReference.target = tocId;
            m_standardReferences.insert(EpubPageReference::TableOfContents, tocReference);
        }

        QDomNodeList spineItemList = spineElement.elementsByTagName("itemref"_L1);
        for (int j = 0; j < spineItemList.count(); j++) {
            parseSpineItem(spineItemList.at(j));
        }
    }

    // Parse out standard items
    QDomNodeList guideNodeList = document.elementsByTagName("guide"_L1);
    for (int i = 0; i < guideNodeList.count(); i++) {
        QDomElement guideElement = guideNodeList.at(i).toElement();

        QDomNodeList guideItemList = guideElement.elementsByTagName("reference"_L1);
        for (int j = 0; j < guideItemList.count(); j++) {
            parseGuideItem(guideItemList.at(j));
        }
    }

    return true;
}

bool EPubContainer::parseMetadataPropertyItem(const QDomElement &metadataElement, const QDomNodeList &nodeList)
{
    if (metadataElement.attribute("property"_L1) == "belongs-to-collection"_L1) {
        const QString id = u'#' + metadataElement.attribute("id"_L1);
        const QString name = metadataElement.text();
        Collection::Type type = Collection::Type::Unknow;
        size_t position = 0;

        if (id.length() == 1) {
            m_collections.append(Collection{name, type, position});
            return true;
        }

        for (int i = 0; i < nodeList.size(); i++) {
            const auto node = nodeList.at(i);
            const auto element = node.toElement();
            if (element.tagName() != "meta"_L1) {
                continue;
            }

            if (element.attribute("refines"_L1) != id) {
                continue;
            }

            if (element.attribute("property"_L1) == "collection-type"_L1) {
                const auto typeString = element.text();
                if (typeString == "set"_L1) {
                    type = Collection::Type::Set;
                } else if (typeString == "series"_L1) {
                    type = Collection::Type::Series;
                }
                continue;
            }

            if (element.attribute("property"_L1) == "group-position"_L1) {
                position = element.text().toInt();
                continue;
            }
        }

        m_collections.append(Collection{name, type, position});
        return true;
    }

    return false;
}

bool EPubContainer::parseMetadataItem(const QDomNode &metadataNode, const QDomNodeList &nodeList)
{
    QDomElement metadataElement = metadataNode.toElement();
    QString tagName = metadataElement.tagName();

    QString metaName;
    QString metaValue;

    if (tagName == "meta"_L1) {
        bool foundProperty = parseMetadataPropertyItem(metadataElement, nodeList);
        if (foundProperty) {
            return true;
        }
        metaName = metadataElement.attribute("name"_L1);
        metaValue = metadataElement.attribute("content"_L1);
    } else if (metadataElement.prefix() != "dc"_L1) {
        qCInfo(KFILEMETADATA_LOG) << "Unsupported metadata tag" << tagName;
        return false;
    } else if (tagName == "date"_L1) {
        metaName = metadataElement.attribute("event"_L1);
        if (metaName.isEmpty()) {
            metaName = tagName;
        }
        metaValue = metadataElement.text();
    } else {
        metaName = tagName;
        metaValue = metadataElement.text();
    }

    if (metaName.isEmpty() || metaValue.isEmpty()) {
        return false;
    }

    if (metaName != "subject"_L1) {
        m_metadata[metaName].append(metaValue);
        return true;
    }

    if (metaValue.contains("--"_L1)) {
        const auto metaValues = metaValue.split("--"_L1);
        if (metaValues.count() <= 1) {
            return false;
        }

        metaValue = metaValues[metaValues.count() - 1].trimmed();
    }

    if (!m_metadata[metaName].contains(metaValue)) {
        m_metadata[metaName].append(metaValue);
        return true;
    }

    return false;
}

bool EPubContainer::parseManifestItem(const QDomNode &manifestNode, const QString &currentFolder)
{
    const QDomElement manifestElement = manifestNode.toElement();
    const QString id = manifestElement.attribute("id"_L1);
    QString path = manifestElement.attribute("href"_L1);
    const QString type = manifestElement.attribute("media-type"_L1);

    if (id.isEmpty() || path.isEmpty()) {
        qCWarning(KFILEMETADATA_LOG) << "Invalid item at line" << manifestElement.lineNumber();
        return false;
    }

    // Resolve relative paths
    path = QDir::cleanPath(currentFolder + path);

    EpubItem item;
    item.mimetype = type.toUtf8();
    item.path = path;
    item.properties = manifestElement.attribute("properties"_L1);
    m_items[id] = item;

    static const auto documentTypes = std::to_array<QString>({
        u"text/x-oeb1-document"_s,
        u"application/x-dtbook+xml"_s,
        u"application/xhtml+xml"_s
    });
    // All items not listed in the spine should be in this
    if (std::find(documentTypes.cbegin(), documentTypes.cend(), type) != documentTypes.cend()) {
        m_unorderedItems.insert(id);
    }

    return true;
}

bool EPubContainer::parseSpineItem(const QDomNode &spineNode)
{
    QDomElement spineElement = spineNode.toElement();

    // Ignore this for now
    if (spineElement.attribute("linear"_L1) == "no"_L1) {
        //        return true;
    }

    QString referenceName = spineElement.attribute("idref"_L1);
    if (referenceName.isEmpty()) {
        qCWarning(KFILEMETADATA_LOG) << "Invalid spine item at line" << spineNode.lineNumber();
        return false;
    }

    if (!m_items.contains(referenceName)) {
        qCWarning(KFILEMETADATA_LOG) << "Unable to find" << referenceName << "in items";
        return false;
    }

    m_unorderedItems.remove(referenceName);
    m_orderedItems.append(referenceName);

    return true;
}

bool EPubContainer::parseGuideItem(const QDomNode &guideItem)
{
    const QDomElement guideElement = guideItem.toElement();
    const QString target = guideElement.attribute("href"_L1);
    const QString title = guideElement.attribute("title"_L1);
    const QString type = guideElement.attribute("type"_L1);

    if (target.isEmpty() || title.isEmpty() || type.isEmpty()) {
        m_errorString = u"Invalid guide item %1 %2 %3"_s.arg(target, title, type);
        return false;
    }

    EpubPageReference reference;
    reference.target = target;
    reference.title = title;

    EpubPageReference::StandardType standardType = EpubPageReference::typeFromString(type);
    if (standardType == EpubPageReference::Other) {
        m_otherReferences[type] = reference;
    } else {
        m_standardReferences[standardType] = reference;
    }

    return true;
}

const KArchiveFile *EPubContainer::file(const QString &path)
{
    if (path.isEmpty()) {
        return nullptr;
    }

    const KArchiveDirectory *folder = m_rootFolder;

    // Try to walk down the correct path
    QStringList pathParts = path.split(QLatin1Char('/'), Qt::SkipEmptyParts);
    for (int i = 0; i < pathParts.count() - 1; i++) {
        QString folderName = pathParts[i];
        const KArchiveEntry *entry = folder->entry(folderName);
        if (!entry) {
            qCWarning(KFILEMETADATA_LOG) << "Unable to find folder name" << folderName << "in" << path.left(100);
            const QStringList entries = folder->entries();
            for (const QString &folderEntry : entries) {
                if (folderEntry.compare(folderName, Qt::CaseInsensitive) == 0) {
                    entry = folder->entry(folderEntry);
                    break;
                }
            }

            if (!entry) {
                qCWarning(KFILEMETADATA_LOG) << "Didn't even find with case-insensitive matching";
                return nullptr;
            }
        }

        if (!entry->isDirectory()) {
            qCWarning(KFILEMETADATA_LOG) << "Expected" << folderName << "to be a directory in path" << path;
            return nullptr;
        }

        folder = dynamic_cast<const KArchiveDirectory *>(entry);
        Q_ASSERT(folder);
    }

    QString filename;
    if (pathParts.isEmpty()) {
        filename = path;
    } else {
        filename = pathParts.last();
    }

    const KArchiveFile *file = folder->file(filename);
    if (!file) {
        qCWarning(KFILEMETADATA_LOG) << "Unable to find file" << filename << "in" << folder->name();

        const QStringList entries = folder->entries();
        for (const QString &folderEntry : entries) {
            if (folderEntry.compare(filename, Qt::CaseInsensitive) == 0) {
                file = folder->file(folderEntry);
                break;
            }
        }

        if (!file) {
            qCWarning(KFILEMETADATA_LOG) << "Unable to find file" << filename << "in" << folder->name() << "with case-insensitive matching" << entries;
        }
    }
    return file;
}

EpubPageReference::StandardType EpubPageReference::typeFromString(const QStringView &name)
{
    if (name == "cover"_L1) {
        return CoverPage;
    } else if (name == "title-page"_L1) {
        return TitlePage;
    } else if (name == "toc"_L1) {
        return TableOfContents;
    } else if (name == "index"_L1) {
        return Index;
    } else if (name == "glossary"_L1) {
        return Glossary;
    } else if (name == "acknowledgements"_L1) {
        return Acknowledgements;
    } else if (name == "bibliography"_L1) {
        return Bibliography;
    } else if (name == "colophon"_L1) {
        return Colophon;
    } else if (name == "copyright-page"_L1) {
        return CopyrightPage;
    } else if (name == "dedication"_L1) {
        return Dedication;
    } else if (name == "epigraph"_L1) {
        return Epigraph;
    } else if (name == "foreword"_L1) {
        return Foreword;
    } else if (name == "loi"_L1) {
        return ListOfIllustrations;
    } else if (name == "lot"_L1) {
        return ListOfTables;
    } else if (name == "notes"_L1) {
        return Notes;
    } else if (name == "preface"_L1) {
        return Preface;
    } else if (name == "text"_L1) {
        return Text;
    } else {
        return Other;
    }
}

QList<Collection> EPubContainer::collections() const
{
    return m_collections;
}

QString EPubContainer::errorString() const
{
    return m_errorString;
}

#include "moc_epubcontainer.cpp"
