/*
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "csvdecoder_p.h"
#include "csvextractor.h"
#include "detecttextcodec_p.h"
#include "kfilemetadata_debug.h"

#include <QFile>

#if defined(Q_OS_LINUX) || defined(__GLIBC__)
    #include <fcntl.h>
#endif

using namespace KFileMetaData;

CsvExtractor::CsvExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{
}

const QStringList supportedMimeTypes = {
    QStringLiteral("text/csv"),
};

QStringList CsvExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void CsvExtractor::extract(ExtractionResult* result)
{
    QFile file(result->inputUrl());

#ifdef O_NOATIME
    const QByteArray filePath = QFile::encodeName(result->inputUrl());
    int fd = open(filePath.constData(), O_RDONLY | O_NOATIME);
    if (fd >= 0) {
        if (!file.open(fd, QIODevice::ReadOnly, QFileDevice::AutoCloseHandle)) {
            return;
        }
    } else
#endif
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    result->addType(Type::Document);
    result->addType(Type::Spreadsheet);
    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    constexpr int32_t chunkSize{256 * 1024};
    QByteArray chunk(chunkSize, Qt::Uninitialized);

    QByteArrayView chunkData = [&file, &chunk]() {
        // read() returns [0...chunkSize] or -1 (error), so no narrowing when casting from qint64
        const auto size = static_cast<int32_t>(file.read(chunk.data(), chunkSize));
        return QByteArrayView{chunk.data(), size};
    }();

    QStringDecoder codec{Helper::autodetectCodec(chunkData)};

    QString text = codec.decode(chunkData);
    if (codec.hasError()) {
        qCDebug(KFILEMETADATA_LOG) << "Invalid" << codec.name() << "encoding. Ignoring" << result->inputUrl();
        return;
    }

    // Sanity check
    if (!Helper::looksLikeText(QStringView{text}.mid(0, 512))) {
        return;
    }

    const auto csvStyle = Helper::detectCsvStyle(text);
    qCDebug(KFILEMETADATA_LOG) << "Extracting" << codec.name() << "CSV data from" << result->inputUrl();

    qsizetype rowCount{0};
    while (!file.atEnd()) {
        auto [rows, end] = Helper::decodeCsv(text, csvStyle);

        if (end == text.size()) {
            text.clear();
        } else {
            rows.removeLast(); // remove partial row
            text.slice(end);
        }

        rowCount += rows.size();
        for (const auto& row : rows) {
            for (const auto& field : row) {
                result->append(field);
            }
            result->append(QStringLiteral("\n"));
        }

        const auto size = static_cast<int32_t>(file.read(chunk.data(), chunkSize));
        if (size < 0) {
            // may happen when the file is truncated during read
            qCWarning(KFILEMETADATA_LOG) << "Error reading" << result->inputUrl();
            break;
        } else if (size == 0) {
            break;
        }

        text += codec({chunk.data(), size});

        if (codec.hasError()) {
            qCDebug(KFILEMETADATA_LOG) << "Invalid encoding. Ignoring" << result->inputUrl();
            return;
        }
    }

    if (!text.isEmpty()) {
        const auto [rows, end] = Helper::decodeCsv(text, csvStyle);

        rowCount += rows.size();
        for (const auto& row : rows) {
            for (const auto& field : row) {
                result->append(field);
            }
            result->append(QStringLiteral("\n"));
        }
    }

    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        result->add(Property::LineCount, rowCount);
    }
}

#include "moc_csvextractor.cpp"
