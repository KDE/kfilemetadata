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
#include <QTextCodec>
#include <QDebug>

#include <fstream>

using namespace KFileMetaData;

PlainTextExtractor::PlainTextExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}

QStringList PlainTextExtractor::mimetypes() const
{
    return QStringList() << QStringLiteral("text/");
}

void PlainTextExtractor::extract(ExtractionResult* result)
{
    std::string line;
    int lines = 0;

    std::ifstream fstream(QFile::encodeName(result->inputUrl()));
    if (!fstream.is_open()) {
        return;
    }

    result->addType(Type::Text);
    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    QTextCodec* codec = QTextCodec::codecForLocale();
    while (std::getline(fstream, line)) {
        QByteArray arr = QByteArray::fromRawData(line.c_str(), line.size());

        QTextCodec::ConverterState state;
        QString text = codec->toUnicode(arr.constData(), arr.size(), &state);

        if (state.invalidChars > 0) {
            qDebug() << "Invalid encoding. Ignoring" << result->inputUrl();
            return;
        }
        result->append(text);

        lines += 1;
    }

    result->add(Property::LineCount, lines);
}
