/*
    SPDX-FileCopyrightText: 2013 Vishesh Handa <me@vhanda.in>
    SPDX-FileCopyrightText: 2016 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "epubextractor.h"

#include <epub.h>

#include <QDateTime>
#include <QRegularExpression>
#include <QDebug>

using namespace KFileMetaData;

EPubExtractor::EPubExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}

namespace
{
static const QStringList supportedMimeTypes = {
    QStringLiteral("application/epub+zip"),
};

QString fetchMetadata(struct epub* e, const epub_metadata& type)
{
    int size = 0;
    unsigned char** data = epub_get_metadata(e, type, &size);
    if (data) {
        QStringList strList;
        for (int i = 0; i < size; i++) {
            // skip nullptr entries, can happen for broken xml files
            if (!data[i])
                continue;

            strList << QString::fromUtf8((char*)data[i]);
            free(data[i]);
        }
        free(data);

        return strList.join(QLatin1String(", "));
    }
    return QString();
}
}

QStringList EPubExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void EPubExtractor::extract(ExtractionResult* result)
{
    // open epub, return on exit, file will be closed again at end of function
    auto ePubDoc = epub_open(result->inputUrl().toUtf8().constData(), 1);
    if (!ePubDoc) {
        qWarning() << "Invalid document";
        return;
    }

    result->addType(Type::Document);

    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {

        QString value = fetchMetadata(ePubDoc, EPUB_TITLE);
        if (!value.isEmpty()) {
            result->add(Property::Title, value);
        }

        value = fetchMetadata(ePubDoc, EPUB_SUBJECT);
        if (!value.isEmpty()) {
            result->add(Property::Subject, value);
        }

        value = fetchMetadata(ePubDoc, EPUB_CREATOR);
        if (!value.isEmpty()) {
            if (value.startsWith(QLatin1String("aut:"), Qt::CaseInsensitive)) {
                value = value.mid(4).simplified();
            } else if (value.startsWith(QLatin1String("author:"), Qt::CaseInsensitive)) {
                value = value.mid(7).simplified();
            }

            // A lot of authors have their name written in () again. We discard that part
            int index = value.indexOf(QLatin1Char('('));
            if (index)
                value = value.mid(0, index);

            result->add(Property::Author, value);
        }

        // The Contributor just seems to be mostly Calibre aka the Generator
        /*
    value = fetchMetadata(ePubDoc, EPUB_CONTRIB);
    if( !value.isEmpty() ) {
        SimpleResource con;
        con.addType( NCO::Contact() );
        con.addProperty( NCO::fullname(), value );

        fileRes.addProperty( NCO::contributor(), con );
        graph << con;
    }*/

        value = fetchMetadata(ePubDoc, EPUB_PUBLISHER);
        if (!value.isEmpty()) {
            result->add(Property::Publisher, value);
        }

        value = fetchMetadata(ePubDoc, EPUB_DESCRIPTION);
        if (!value.isEmpty()) {
            result->add(Property::Description, value);
        }

        value = fetchMetadata(ePubDoc, EPUB_DATE);
        if (!value.isEmpty()) {
            if (value.startsWith(QLatin1String("Unspecified:"), Qt::CaseInsensitive)) {
                value = value.mid(QByteArray("Unspecified:").size()).simplified();
            }
            int ind = value.indexOf(QLatin1String("publication:"), Qt::CaseInsensitive);
            if (ind != -1) {
                value = value.mid(ind + QByteArray("publication:").size()).simplified();
            }
            QDateTime dt = ExtractorPlugin::dateTimeFromString(value);
            if (!dt.isNull()) {
                result->add(Property::CreationDate, dt);
                result->add(Property::ReleaseYear, dt.date().year());
            }
        }
    }

    //
    // Plain Text
    //
    if (result->inputFlags() & ExtractionResult::ExtractPlainText) {
        if (auto iter = epub_get_iterator(ePubDoc, EITERATOR_SPINE, 0)) {
            do {
                char* curr = epub_it_get_curr(iter);
                if (!curr)
                    continue;

                QString html = QString::fromUtf8(curr);
                html.remove(QRegularExpression(QStringLiteral("<[^>]*>")));
                result->append(html);
            } while (epub_it_get_next(iter));

            epub_free_iterator(iter);
        }

        auto tit = epub_get_titerator(ePubDoc, TITERATOR_NAVMAP, 0);
        if (!tit) {
            tit = epub_get_titerator(ePubDoc, TITERATOR_GUIDE, 0);
        }
        if (tit) {
            if (epub_tit_curr_valid(tit)) {
                do {
                    // get link, iterator handles freeing of it
                    char* clink = epub_tit_get_curr_link(tit);

                    // epub_get_data returns -1 on failure
                    char* data = nullptr;
                    const int size = epub_get_data(ePubDoc, clink, &data);
                    if (size >= 0 && data) {
                        QString html = QString::fromUtf8(data, size);
                        // strip html tags
                        html.remove(QRegularExpression(QStringLiteral("<[^>]*>")));

                        result->append(html);
                        free(data);
                    }
                } while (epub_tit_next(tit));
            }
            epub_free_titerator(tit);
        }
    }

    // close epub file again
    epub_close(ePubDoc);
}
