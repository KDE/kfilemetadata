/*
    SPDX-FileCopyrightText: 2013 Vishesh Handa <me@vhanda.in>
    SPDX-FileCopyrightText: 2016 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "datetimeparser_p.h"
#include "epubextractor.h"
#include "epubcontainer.h"
#include "kfilemetadata_debug.h"

#include <QDateTime>
#include <QImage>
#include <QRegularExpression>

using namespace KFileMetaData;
using namespace Qt::StringLiterals;

EPubExtractor::EPubExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{
}

namespace
{
static const QStringList supportedMimeTypes = {
    QStringLiteral("application/epub+zip"),
};

}

QStringList EPubExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void EPubExtractor::extract(ExtractionResult* result)
{
    EPubContainer epub;
    if (!epub.openFile(result->inputUrl())) {
        qCWarning(KFILEMETADATA_LOG) << epub.errorString();
        return;
    }

    result->addType(Type::Document);

    // Metadata
    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        for (const QString &value : epub.metadata(u"identifier"_s)) {
            result->add(Property::Identifier, value);
        }

        for (const QString &value : epub.metadata(u"title"_s)) {
            result->add(Property::Title, value);
        }

        for (const QString &value : epub.metadata(u"subject"_s)) {
            result->add(Property::Subject, value);
        }

        for (const QString &value : epub.metadata(u"language"_s)) {
            result->add(Property::Language, value);
        }

        for (const QString &value: epub.metadata(u"creator"_s)) {
            result->add(Property::Author, value);
        }

        for (const QString &value: epub.metadata(u"contributor"_s)) {
            result->add(Property::Generator, value);
        }

        for (const QString &value : epub.metadata(u"publisher"_s)) {
            result->add(Property::Publisher, value);
        }

        for (const QString &value : epub.metadata(u"se:word-count"_s)) {
            bool ok = false;
            const auto wordCount = value.toInt(&ok);
            if (ok) {
                result->add(Property::WordCount, wordCount);
            }
        }

        for (const QString &value : epub.metadata(u"rights"_s)) {
            result->add(Property::License, value);
        }

        bool foundPublicationDate = false;
        for (const QString &value : epub.metadata(u"publication"_s)) {
            QDateTime dt = Parser::dateTimeFromString(value);
            if (dt.timeZone().timeSpec() == Qt::LocalTime) {
                dt.setTimeZone(QTimeZone::UTC);
            }

            if (!dt.isNull()) {
                foundPublicationDate = true;
                result->add(Property::CreationDate, dt);
                result->add(Property::ReleaseYear, dt.date().year());
            }
        }

        if (!foundPublicationDate) {
            for (const QString &value : epub.metadata(u"date"_s)) {
                QDateTime dt = Parser::dateTimeFromString(value);
                if (dt.timeZone().timeSpec() == Qt::LocalTime) {
                    dt.setTimeZone(QTimeZone::UTC);
                }

                if (!dt.isNull()) {
                    result->add(Property::CreationDate, dt);
                    result->add(Property::ReleaseYear, dt.date().year());
                }
            }
        }

        for (const QString &value : epub.metadata(u"description"_s)) {
            auto html = value;
            html.remove(QRegularExpression(u"<[^>]*>"_s));
            result->add(Property::Description, html);
        }

        for (const QString &value : epub.metadata(u"source"_s)) {
            result->add(Property::OriginUrl, value);
        }

        bool foundCover = false;
        for (const QString &value : epub.metadata(u"cover"_s)) {
            auto image = epub.image(value);
            if (!image.isNull()) {
                result->add(Property::Cover, image);
                foundCover = true;
                break;
            }
        }

        if (!foundCover) {
            const QStringList items = epub.items();
            for (const auto &item : items) {
                const auto epubItem = epub.epubItem(item);
                if (epubItem.properties.contains("cover-image"_L1)) {
                    auto image = epub.image(item);
                    if (!image.isNull()) {
                        result->add(Property::Cover, image);
                        foundCover = true;
                        break;
                    }
                }
            }
        }

        const auto collections = epub.collections();
        for (const auto &collection : collections) {
            result->add(Property::Serie, collection.name);
            result->add(Property::VolumeNumber, QString::number(collection.position));
        }
    }

    // Plain Text
    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    QStringList items = epub.items();

    const QString cover = epub.standardPage(EpubPageReference::CoverPage);
    if (!cover.isEmpty()) {
        items.prepend(cover);
    }

    while(!items.isEmpty()) {
        const QString &chapter = items.takeFirst();
        const auto currentItem = epub.epubItem(chapter);

        if (currentItem.path.isEmpty()) {
            continue;
        }

        auto ioDevice = epub.ioDevice(currentItem.path);
        if (!ioDevice) {
            qCWarning(KFILEMETADATA_LOG) << "Unable to get iodevice for chapter" << chapter;
            continue;
        }


        auto html = QString::fromUtf8(ioDevice->readAll());
        html.remove(QRegularExpression(u"<[^>]*>"_s));
        result->append(html);
    }
}

#include "moc_epubextractor.cpp"
