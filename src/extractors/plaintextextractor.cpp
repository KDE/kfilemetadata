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

namespace {

bool looksLikeText(QStringView text)
{
    std::array<uint16_t, 7> groups{0};
    for (const QChar c : text) {
        if (auto category = c.category(); category > 29) {
            continue;
        } else if (category <= 2) { // Mark
            groups[0]++;
        } else if (category <= 5) { // Number
            groups[1]++;
        } else if (category <= 8) { // Separator
            groups[2]++;
        } else if (category <= 13) { // Other
            groups[3]++;
        } else if (category <= 18) { // Letter
            groups[4]++;
        } else if (category <= 25) { // Punctuation
            groups[5]++;
        } else if (category <= 29) { // Symbol
            groups[6]++;
        }
    }
    return (groups[0] + groups[3] + groups[6]) < (text.size() / 2);
}
} // namespace <anonymous>

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

    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        result->addType(Type::Text);
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
    constexpr int32_t chunkSize{256 * 1024};
    QByteArray chunk(chunkSize, Qt::Uninitialized);

    QByteArrayView chunkData = [&file, &chunk]() {
        // read() returns [0...chunkSize] or -1 (error), so no narrowing when casting from qint64
        const auto size = static_cast<int32_t>(file.read(chunk.data(), chunkSize));
        return QByteArrayView{chunk.data(), size};
    }();

    QStringDecoder codec{autodetectCodec(chunkData)};

    QString text = codec.decode(chunkData);
    if (codec.hasError()) {
        qCDebug(KFILEMETADATA_LOG) << "Invalid" << codec.name() << "encoding. Ignoring" << result->inputUrl();
        return;
    }

    // Sanity check
    if (!looksLikeText(QStringView{text}.mid(0, 512))) {
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
        const auto size = static_cast<int32_t>(file.read(chunk.data(), chunkSize));
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

    result->addType(Type::Text);
    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        result->add(Property::LineCount, lines);
    }
}

#include "moc_plaintextextractor.cpp"
