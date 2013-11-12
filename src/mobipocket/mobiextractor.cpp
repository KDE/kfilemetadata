/*
    Copyright (C) 2013  Vishesh Handa <me@vhanda.in>

    Code adapted from kdegraphics-mobipocket/strigi/
    Copyright (C) 2008 by Jakub Stachowski <qbast@go2.pl>

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


#include "mobiextractor.h"

#include <qmobipocket/mobipocket.h>
#include "qfilestream.h"

#include <KDebug>
#include <QtCore/QDateTime>
#include <QTextDocument>

using namespace KMetaData;

MobiExtractor::MobiExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

QStringList MobiExtractor::mimetypes()
{
    QStringList types;
    types << QLatin1String("application/x-mobipocket-ebook");

    return types;
}

QVariantMap MobiExtractor::extract(const QString& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    QVariantMap metadata;

    Mobipocket::QFileStream stream(fileUrl);
    Mobipocket::Document doc(&stream);
    if (!doc.isValid())
        return metadata;

    QMapIterator<Mobipocket::Document::MetaKey, QString> it(doc.metadata());
    while (it.hasNext()) {
        it.next();
        switch (it.key()) {
        case Mobipocket::Document::Title:
            metadata.insert("title", it.value());
            break;
        case Mobipocket::Document::Author: {
            metadata.insert("author", it.value());
            break;
        }
        case Mobipocket::Document::Description: {
            QTextDocument document;
            document.setHtml(it.value());

            QString plain = document.toPlainText();
            if (!plain.isEmpty())
                metadata.insert("comment", it.value());
            break;
        }
        case Mobipocket::Document::Subject:
            metadata.insert("subject", it.value());
            break;
        case Mobipocket::Document::Copyright:
            metadata.insert("copyright", it.value());
            break;
        }
    }

    if (!doc.hasDRM()) {
        QString html = doc.text(maxPlainTextSize());

        QTextDocument document;
        document.setHtml(html);

        QString plainText = document.toPlainText();
        if (!plainText.isEmpty())
            metadata.insert("text", it.value());
    }

    return metadata;
}

KMETADATA_EXPORT_EXTRACTOR(KMetaData::MobiExtractor, "nepomukmobiextractor")
