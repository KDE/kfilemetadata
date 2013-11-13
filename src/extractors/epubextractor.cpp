/*
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


#include "epubextractor.h"

#include <epub.h>

#include <KDebug>
#include <QtCore/QDateTime>
#include <QTextDocument>


using namespace KMetaData;

EPubExtractor::EPubExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

QStringList EPubExtractor::mimetypes()
{
    QStringList types;
    types << QLatin1String("application/epub+zip");

    return types;
}

namespace
{
QString fetchMetadata(struct epub* e, const epub_metadata& type)
{
    int size = 0;

    unsigned char** data = epub_get_metadata(e, type, &size);
    if (data) {
        QStringList strList;
        for (int i = 0; i < size; i++) {
            strList << QString::fromUtf8((char*)data[i]);
            free(data[i]);
        }
        free(data);

        return strList.join(";");
    }
    return QString();
}
}


QVariantMap EPubExtractor::extract(const QString& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    struct epub* ePubDoc = epub_open(fileUrl.toUtf8().constData(), 1);
    if (!ePubDoc) {
        kError() << "Invalid document";
        return QVariantMap();
    }

    QVariantMap metadata;

    QString value = fetchMetadata(ePubDoc, EPUB_TITLE);
    if (!value.isEmpty()) {
        metadata.insert("title", value);
    }

    value = fetchMetadata(ePubDoc, EPUB_SUBJECT);
    if (!value.isEmpty()) {
        metadata.insert("subject", value);
    }

    value = fetchMetadata(ePubDoc, EPUB_CREATOR);
    if (!value.isEmpty()) {
        if (value.startsWith(QLatin1String("aut:"), Qt::CaseInsensitive)) {
            value = value.mid(4).simplified();
        } else if (value.startsWith(QLatin1String("author:"), Qt::CaseInsensitive)) {
            value = value.mid(7).simplified();
        }

        // A lot of authors have their name written in () again. We discard that part
        int index = value.indexOf('(');
        if (index)
            value = value.mid(0, index);

        metadata.insert("creator", value);
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
        metadata.insert("publisher", value);
    }

    value = fetchMetadata(ePubDoc, EPUB_DESCRIPTION);
    if (!value.isEmpty()) {
        metadata.insert("description", value);
    }

    value = fetchMetadata(ePubDoc, EPUB_DATE);
    if (!value.isEmpty()) {
        if (value.startsWith("Unspecified:", Qt::CaseInsensitive)) {
            value = value.mid(QString("Unspecified:").size()).simplified();
        }
        int ind = value.indexOf("publication:", Qt::CaseInsensitive);
        if (ind != -1) {
            value = value.mid(ind + QString("publication:").size()).simplified();
        }
        QDateTime dt = ExtractorPlugin::dateTimeFromString(value);
        if (!dt.isNull())
            metadata.insert("creationDate", value);
    }

    //
    // Plain Text
    //

    QString plainText;

    struct eiterator* iter = epub_get_iterator(ePubDoc, EITERATOR_SPINE, 0);
    do {
        char* curr = epub_it_get_curr(iter);
        if (!curr)
            continue;
        QString html = QString::fromUtf8(curr);

        QTextDocument doc;
        doc.setHtml(html);
        plainText.append(doc.toPlainText() + "\n");
    } while (epub_it_get_next(iter));

    epub_free_iterator(iter);

    struct titerator* tit;

    tit = epub_get_titerator(ePubDoc, TITERATOR_NAVMAP, 0);
    if (!tit) {
        tit = epub_get_titerator(ePubDoc, TITERATOR_GUIDE, 0);
    }

    if (epub_tit_curr_valid(tit)) {
        do {
            char* clink = epub_tit_get_curr_link(tit);

            char* data;
            int size = epub_get_data(ePubDoc, clink, &data);
            free(clink);

            // epub_get_data returns -1 on failure
            if (size > 0 && data) {
                QString html = QString::fromUtf8(data, size);

                QTextDocument doc;
                doc.setHtml(html);
                plainText.append(doc.toPlainText() + "\n");
                free(data);
            }
        } while (epub_tit_next(tit));
    }
    epub_free_titerator(tit);

    if (!plainText.isEmpty())
        metadata.insert("text", plainText);

    return metadata;
}

KFILEMETADATA_EXPORT_EXTRACTOR(KMetaData::EPubExtractor, "kfilemetadata_epubextractor")
