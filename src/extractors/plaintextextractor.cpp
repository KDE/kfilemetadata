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

QStringList PlainTextExtractor::mimetypes() const
{
    return QStringList() << QLatin1String("text/");
}

// TODO: Make this iterative! And remove the size filter
void PlainTextExtractor::extract(ExtractionResult* result)
{
    QFile file(result->inputUrl());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    int lines = 0;
    int words = 0;

    QRegExp wordsRegex("\\b\\w+\\b");

    QTextStream ts(&file);
    while (!ts.atEnd()) {
        QString str = ts.readLine();
        result->append(str);

        lines += 1;
        words += str.count(wordsRegex);
    }

    result->add(Property::WordCount, words);
    result->add(Property::LineCount, lines);
    result->addType(Type::Text);

    return;
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::PlainTextExtractor, "kfilemetadata_plaintextextractor")
