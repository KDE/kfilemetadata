/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "plaintextextractor.h"

#include <QFile>
#include <QTextCodec>
#include <QDebug>

#include <fstream>

#if defined(Q_OS_LINUX) || defined(__GLIBC__)
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

using namespace KFileMetaData;

PlainTextExtractor::PlainTextExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}

const QStringList supportedMimeTypes = {
    QStringLiteral("text/plain"),
};

QStringList PlainTextExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void PlainTextExtractor::extract(ExtractionResult* result)
{
#if defined(Q_OS_LINUX) || defined(__GLIBC__)
    QByteArray filePath = QFile::encodeName(result->inputUrl());

#ifdef O_NOATIME
    int fd = open(filePath.constData(), O_RDONLY | O_NOATIME);
    if (fd < 0)
#else
    int fd;
#endif
    {
        fd = open(filePath.constData(), O_RDONLY);
    }

    if (fd < 0) {
        return;
    }

    result->addType(Type::Text);
    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        close(fd);
        return;
    }

    QTextCodec* codec = QTextCodec::codecForLocale();

    char* line = nullptr;
    size_t len = 0;
    int lines = 0;
    int r = 0;

    FILE* fp = fdopen(fd, "r");

    while ( (r = getline(&line, &len, fp)) != -1) {
        QTextCodec::ConverterState state;
        QString text = codec->toUnicode(line, r - 1, &state);

        if (state.invalidChars > 0) {
            qDebug() << "Invalid encoding. Ignoring" << result->inputUrl();
            free(line);
            close(fd);
            return;
        }
        result->append(text);

        lines += 1;
    }
    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        result->add(Property::LineCount, lines);
    }

    free(line);
    close(fd);

#else
    std::string line;
    int lines = 0;

    std::ifstream fstream(QFile::encodeName(result->inputUrl()).constData());
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
#endif
}
