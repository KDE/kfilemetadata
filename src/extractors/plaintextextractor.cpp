/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>

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


#include "plaintextextractor.h"

#include <QtCore/QFile>
#include <KDebug>

using namespace KFileMetaData;

PlainTextExtractor::PlainTextExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

QStringList PlainTextExtractor::mimetypes()
{
    return QStringList() << QLatin1String("text/");
}

QVariantMap PlainTextExtractor::extract(const QString& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    QFile file(fileUrl);
    QVariantMap metadata;

    // FIXME: make a size filter or something configurable
    if (file.size() > 5 * 1024 * 1024) {
        return metadata;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return metadata;
    }

    QTextStream ts(&file);
    QString contents = ts.readAll();

    int characters = contents.length();
    int lines = contents.count(QChar('\n'));
    int words = contents.count(QRegExp("\\b\\w+\\b"));

    metadata.insert("type", "PlainTextDocument");
    metadata.insert("text", contents);
    metadata.insert("wordCount", words);
    metadata.insert("lines", lines);
    metadata.insert("characterCount", characters);

    return metadata;
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::PlainTextExtractor, "kfilemetadata_plaintextextractor")
