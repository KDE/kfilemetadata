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

#include <QFile>
#include <QTextStream>

using namespace KFileMetaData;

namespace {
inline bool isWordCharacter(const QChar& c)
{
    // The Qt docs say for word characters:
    //      \w  - Matches a word character (QChar::isLetterOrNumber(), QChar::isMark(), or '_').
    // see also: http://qt-project.org/doc/qt-4.8/qregexp.html
    return c.isLetterOrNumber() || c.isMark() || c.unicode() == '_';
}

inline int countWords(const QString &string)
{
    int words = 0;
    bool inWord = false;
    foreach(QChar c, string) {
        if (isWordCharacter(c) != inWord) {
            inWord = !inWord;
            if (inWord) {
                ++words;
            }
        }
    }

    return words;
}
}

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

    QTextStream ts(&file);
    while (!ts.atEnd()) {
        QString str = ts.readLine();
        result->append(str);

        lines += 1;
        words += countWords(str);
    }

    result->add(Property::WordCount, words);
    result->add(Property::LineCount, lines);
    result->addType(Type::Text);

    return;
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::PlainTextExtractor, "kfilemetadata_plaintextextractor")
