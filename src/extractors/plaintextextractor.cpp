/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "kfilemetadata_debug.h"
#include "plaintextextractor.h"

#include <QFile>
#include <QStringDecoder>
#include <QtMinMax>

#include <KEncodingProber>

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
        isOpen = file.open(fd, QIODevice::ReadOnly, QFileDevice::AutoCloseHandle);
    } else
#endif
    {
        isOpen = file.open(QIODevice::ReadOnly);
    }

    if (!isOpen) {
        return;
    }

    result->addType(Type::Text);
    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    auto autodetectCodec = [](const QByteArrayView buffer) -> QStringDecoder {

        // First 16 bytes for detecting by BOM.
        const QByteArrayView bufferForBom(buffer.begin(), qMin(16, buffer.size()));

        // first: try to get encoding by BOM handling
        // If BOM has been found, trust it
        if (auto encoding = QStringConverter::encodingForData(bufferForBom)) {
            QStringDecoder decoder(encoding.value());
            return decoder;
        }

        // second: try to get encoding by KEncodingProber
        KEncodingProber prober(KEncodingProber::Universal);
        prober.feed(buffer);

        // we found codec with some confidence?
        if (auto confidence = prober.confidence(); (confidence > 0.5) //
            || ((confidence > 0.1) && (prober.encoding().toLower() == "utf-8"))) {
            auto proberDecoder = QStringDecoder(prober.encoding().constData());
            // rare case, but if not valid, do not return proberDecoder
            if (proberDecoder.isValid()) {
                return proberDecoder;
            }
        }

        return QStringDecoder{QStringConverter::System};
    };

    // Read the first chunk, detect the encoding and decode it
    QByteArray chunk(256 * 1024, Qt::Uninitialized);
    auto size = file.read(chunk.data(), chunk.size());

    QStringDecoder codec{autodetectCodec({chunk.data(), size})};

    QString text = codec.decode({chunk.data(), size});
    if (codec.hasError()) {
        qCDebug(KFILEMETADATA_LOG) << "Invalid" << codec.name() << "encoding. Ignoring" << result->inputUrl();
        return;
    }

    // Detect the end-of-line variant
    const auto eol = [](const QString &text) {
        auto nl = text.indexOf(QLatin1Char('\n'));
        if ((nl >= 1) && (text[nl - 1] == QLatin1Char('\r'))) {
            return QStringLiteral("\r\n");
        } else if (nl >= 0) {
            return QStringLiteral("\n");
        } else if (text.indexOf(QLatin1Char('\r')) >= 0) {
            return QStringLiteral("\r");
        }
        return QStringLiteral("\n");
    }(text);

    qCDebug(KFILEMETADATA_LOG) << "Extracting" << codec.name() << eol << "plain text from" << result->inputUrl();

    // Read and decode the remainder
    while (!file.atEnd()) {
        auto size = file.read(chunk.data(), chunk.size());
        if (size < 0) {
            // may happen when the file is truncated during read
            qCWarning(KFILEMETADATA_LOG) << "Error reading" << result->inputUrl();
            break;
        } else if (size == 0) {
            break;
        }

        text += codec.decode({chunk.data(), size});
        if (codec.hasError()) {
            qCDebug(KFILEMETADATA_LOG) << "Invalid encoding. Ignoring" << result->inputUrl();
            return;
        }
    }

    // Split lines and count
    int lines = 0;
    qsizetype start = 0;
    while (start < text.size()) {
        auto end = text.indexOf(eol, start);
        lines += 1;
        if (end == -1) {
            result->append(text.mid(start));
            break;
        }
        result->append(text.mid(start, end - start));
        start = end + eol.size();
    }

    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        result->add(Property::LineCount, lines);
    }
}

#include "moc_plaintextextractor.cpp"
