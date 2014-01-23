/*
    This file is part of a KMetaData File Extractor
    Copyright (C) 2013  Denis Steckelmacher <steckdenis@yahoo.fr>

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

#include "officeextractor.h"

#include <kstandarddirs.h>

#include <QFile>
#include <QProcess>

using namespace KFileMetaData;

OfficeExtractor::OfficeExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{
    // Find the executables of catdoc, catppt and xls2csv. If an executable cannot
    // be found, indexing its corresponding MIME type will be disabled
    findExe("application/msword", "catdoc", m_catdoc);
    findExe("application/vnd.ms-excel", "xls2csv", m_xls2csv);
    findExe("application/vnd.ms-powerpoint", "catppt", m_catppt);
}

void OfficeExtractor::findExe(const QString& mimeType, const QString& name, QString& fullPath)
{
    fullPath = KStandardDirs::findExe(name);

    if (!fullPath.isEmpty()) {
        m_available_mime_types << mimeType;
    }
}

QStringList OfficeExtractor::mimetypes() const
{
    return m_available_mime_types;
}


void OfficeExtractor::extract(ExtractionResult* result)
{
    QVariantMap metadata;
    QStringList args;
    QString contents;

    args << QLatin1String("-s") << QLatin1String("cp1252"); // FIXME: Store somewhere a map between the user's language and the encoding of the Windows files it may use ?
    args << QLatin1String("-d") << QLatin1String("utf8");

    const QString fileUrl = result->inputUrl();
    const QString mimeType = result->inputMimetype();
    if (mimeType == QLatin1String("application/msword")) {
        result->addType(Type::Document);

        args << QLatin1String("-w");
        contents = textFromFile(fileUrl, m_catdoc, args);

        // Now that we have the plain text content, count words, lines and characters
        // (original code from plaintextextractor.cpp, authored by Vishesh Handa)
        int lines = contents.count(QLatin1Char('\n'));
        int words = contents.count(QRegExp("\\b\\w+\\b"));

        result->add(Property::WordCount, words);
        result->add(Property::LineCount, lines);
    } else if (mimeType == QLatin1String("application/vnd.ms-excel")) {
        result->addType(Type::Document);
        result->addType(Type::Spreadsheet);

        args << QLatin1String("-c") << QLatin1String(" ");
        args << QLatin1String("-b") << QLatin1String(" ");
        args << QLatin1String("-q") << QLatin1String("0");
        contents = textFromFile(fileUrl, m_xls2csv, args);
    } else if (mimeType == QLatin1String("application/vnd.ms-powerpoint")) {
        result->addType(Type::Document);
        result->addType(Type::Presentation);

        contents = textFromFile(fileUrl, m_catppt, args);
    }

    if (contents.isEmpty())
        return;

    result->append(contents);

    return;
}

QString OfficeExtractor::textFromFile(const QString& fileUrl, const QString& command, QStringList& arguments)
{
    arguments << fileUrl;

    // Start a process and read its standard output
    QProcess process;

    process.setReadChannel(QProcess::StandardOutput);
    process.start(command, arguments, QIODevice::ReadOnly);
    process.waitForFinished();

    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0)
        return QString();
    else
        return QString::fromUtf8(process.readAll());
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::OfficeExtractor, "kfilemetadata_officeextractor")
