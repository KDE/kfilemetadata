/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "plaintextextractor.h"

#include <QDebug>
#include <QFile>
#include <QStringDecoder>

#if defined(Q_OS_LINUX) || defined(__GLIBC__)
    #include <fcntl.h>
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
    QFile file(result->inputUrl());
    bool isOpen = false;

#ifdef O_NOATIME
    const QByteArray filePath = QFile::encodeName(result->inputUrl());
    int fd = open(filePath.constData(), O_RDONLY | O_NOATIME);
    if (fd >= 0) {
        isOpen = file.open(fd, QIODevice::ReadOnly | QIODevice::Text, QFileDevice::AutoCloseHandle);
    } else
#endif
    {
        isOpen = file.open(QIODevice::ReadOnly | QIODevice::Text);
    }

    if (!isOpen) {
        return;
    }

    result->addType(Type::Text);
    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    QStringDecoder codec(QStringConverter::System);

    int lines = 0;

    while (!file.atEnd()) {
        QString text = codec.decode(file.readLine());

        if (codec.hasError()) {
            qDebug() << "Invalid encoding. Ignoring" << result->inputUrl();
            return;
        }

        if (!text.isEmpty() && text.back() == QLatin1Char('\n')) {
            text.removeLast();
        }

        result->append(text);

        lines += 1;
    }
    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        result->add(Property::LineCount, lines);
    }
}

#include "moc_plaintextextractor.cpp"
