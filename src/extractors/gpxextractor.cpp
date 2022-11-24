/*
    SPDX-FileCopyrightText: 2022 Kai Uwe Broulik <kde@broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "gpxextractor.h"

#include <QDateTime>
#include <QFile>
#include <QXmlStreamReader>

using namespace KFileMetaData;

GpxExtractor::GpxExtractor(QObject *parent)
    : ExtractorPlugin(parent)
{
}

namespace
{

static const QStringList supportedMimeTypes = {
    QStringLiteral("application/gpx+xml")
};

inline QString gpxNS() { return QStringLiteral("http://www.topografix.com/GPX/1/1"); }

}

QStringList GpxExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void GpxExtractor::extract(ExtractionResult *result)
{
    QFile file(result->inputUrl());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    if (!(result->inputFlags() & ExtractionResult::ExtractMetaData)) {
        return;
    }

    QXmlStreamReader xml(&file);

    bool inGpx = false;
    bool inMetadata = false;
    bool inAuthor = false;
    bool inLink = false;
    bool inText = false;
    bool inTrk = false;

    QString title;
    QString description;
    QString author;

    while (!xml.atEnd() && !xml.hasError()) {
        xml.readNext();

        if (xml.isStartElement()) {
            if (!inGpx && xml.name() == QLatin1String("gpx") && xml.namespaceUri() == gpxNS()) {
                inGpx = true;
            } else if (!inMetadata && xml.name() == QLatin1String("metadata") && xml.namespaceUri() == gpxNS()) {
                inMetadata = true;
            } else if (!inAuthor && xml.name() == QLatin1String("author") && xml.namespaceUri() == gpxNS()) {
                inAuthor = true;
            } else if (!inLink && xml.name() == QLatin1String("link") && xml.namespaceUri() == gpxNS()) {
                inLink = true;
            } else if (!inText && xml.name() == QLatin1String("text") && xml.namespaceUri() == gpxNS()) {
                inText = true;
            } else if (!inTrk && xml.name() == QLatin1String("trk") && xml.namespaceUri() == gpxNS()) {
                inTrk = true;
            }

            if (inMetadata) {
                if (xml.name() == QLatin1String("name")) {
                    title = xml.readElementText();
                } else if (xml.name() == QLatin1String("desc")) {
                    description = xml.readElementText();
                } else if (inAuthor) {
                    if (xml.name() == QLatin1String("name")) {
                        author = xml.readElementText();
                    // Use author link text if no author name is provided.
                    } else if (inLink && inText && author.isEmpty()) {
                        author = xml.readElementText();
                    }
                } else if (xml.name() == QLatin1String("copyright")) {
                    const auto copyrightAuthor = xml.attributes().value(QLatin1String("author"));
                    if (!copyrightAuthor.isEmpty()) {
                        result->add(Property::Copyright, copyrightAuthor.toString());
                    }
                } else if (xml.name() == QLatin1String("time")) {
                    const QDateTime dt = QDateTime::fromString(xml.readElementText(), Qt::ISODate);
                    if (dt.isValid()) {
                        result->add(Property::CreationDate, dt);
                    }
                }
                // TODO keywords
                // TODO use center of "bounds" for GPS coordinates?
            } else if (inTrk) {
                // If no metadata title/description is provided, use first track name we find.
                if (title.isEmpty() && xml.name() == QLatin1String("name")) {
                    title = xml.readElementText();
                } else if (description.isEmpty() && xml.name() == QLatin1String("desc")) {
                    description = xml.readElementText();
                }
            }
        } else if (xml.isEndElement()) {
            if (inGpx && xml.name() == QLatin1String("gpx") && xml.namespaceUri() == gpxNS()) {
                break; // end of document.
            } else if (inMetadata && xml.name() == QLatin1String("metadata") && xml.namespaceUri() == gpxNS()) {
                inMetadata = false;

                if (!title.isEmpty() && !description.isEmpty()) {
                    // We have already found title and description, no need to carry on through the tracks.
                    break;
                }
            } else if (inAuthor && xml.name() == QLatin1String("author") && xml.namespaceUri() == gpxNS()) {
                inAuthor = false;
            } else if (!inLink && xml.name() == QLatin1String("link") && xml.namespaceUri() == gpxNS()) {
                inLink = false;
            } else if (!inLink && xml.name() == QLatin1String("text") && xml.namespaceUri() == gpxNS()) {
                inText = false;
            } else if (inGpx && xml.name() == QLatin1String("gpx") && xml.namespaceUri() == gpxNS()) {
                inTrk = false;
            }
        }
    }

    if (!title.isEmpty()) {
        result->add(Property::Title, title);
    }
    if (!description.isEmpty()) {
        result->add(Property::Description, description);
    }
    if (!author.isEmpty()) {
        result->add(Property::Author, author);
    }
}
