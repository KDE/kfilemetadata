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

#include <KDebug>
#include <QDateTime>
#include <QFile>
#include <QTextDocument>

using namespace KFileMetaData;

class QFileStream : public Mobipocket::Stream
{
public:
    QFileStream(const QString& name) : d(name) {
        d.open(QIODevice::ReadOnly);
    }
    int read(char* buf, int size) {
        return d.read(buf, size);
    }
    bool seek(int pos) {
        return d.seek(pos);
    }
private:
    QFile d;
};

MobiExtractor::MobiExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

QStringList MobiExtractor::mimetypes() const
{
    QStringList types;
    types << QLatin1String("application/x-mobipocket-ebook");

    return types;
}

void MobiExtractor::extract(ExtractionResult* result)
{
    QFileStream stream(result->inputUrl());
    Mobipocket::Document doc(&stream);
    if (!doc.isValid())
        return;

    QMapIterator<Mobipocket::Document::MetaKey, QString> it(doc.metadata());
    while (it.hasNext()) {
        it.next();
        switch (it.key()) {
        case Mobipocket::Document::Title:
            result->add(Property::Title, it.value());
            break;
        case Mobipocket::Document::Author: {
            result->add(Property::Author, it.value());
            break;
        }
        case Mobipocket::Document::Description: {
            QTextDocument document;
            document.setHtml(it.value());

            QString plain = document.toPlainText();
            if (!plain.isEmpty())
                result->add(Property::Description, it.value());
            break;
        }
        case Mobipocket::Document::Subject:
            result->add(Property::Subject, it.value());
            break;
        case Mobipocket::Document::Copyright:
            result->add(Property::Copyright, it.value());
            break;
        }
    }

    if (!doc.hasDRM()) {
        QString html = doc.text();

        QTextDocument document;
        document.setHtml(html);

        result->append(document.toPlainText());
    }

    result->addType(Type::Document);
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::MobiExtractor, "kfilemetadata_mobiextractor")
