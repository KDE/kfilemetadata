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

#include "nie.h"
#include "nfo.h"

#include <QtCore/QFile>
#include <KDebug>

using namespace Nepomuk2::Vocabulary;

namespace Nepomuk2
{

PlainTextExtractor::PlainTextExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

bool PlainTextExtractor::shouldExtract(const QUrl& url, const QString& mimeType)
{
    Q_UNUSED(url);
    return mimeType.startsWith(QLatin1String("text/")) || mimeType.endsWith(QLatin1String("/xml"));
}

SimpleResourceGraph PlainTextExtractor::extract(const QUrl& resUri, const QUrl& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    QFile file(fileUrl.toLocalFile());

    // FIXME: make a size filter or something configurable
    if (file.size() > 5 * 1024 * 1024) {
        return SimpleResourceGraph();
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return SimpleResourceGraph();
    }

    QTextStream ts(&file);
    QString contents = ts.readAll();

    int characters = contents.length();
    int lines = contents.count(QChar('\n'));
    int words = contents.count(QRegExp("\\b\\w+\\b"));

    SimpleResource fileRes(resUri);
    fileRes.addType(NFO::PlainTextDocument());
    fileRes.addProperty(NIE::plainTextContent(), contents);
    fileRes.addProperty(NFO::wordCount(), words);
    fileRes.addProperty(NFO::lineCount(), lines);
    fileRes.addProperty(NFO::characterCount(), characters);

    return SimpleResourceGraph() << fileRes;
}

}

NEPOMUK_EXPORT_EXTRACTOR(Nepomuk2::PlainTextExtractor, "nepomukplaintextextractor")
