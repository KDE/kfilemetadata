/*
    SPDX-FileCopyrightText: 2024 Sergey Katunin <sulmpx60@yandex.ru>
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef DETECTTEXTCODEC_P_H
#define DETECTTEXTCODEC_P_H

#include <QStringDecoder>
#include <QtMinMax>

#include <KEncodingProber>

namespace KFileMetaData
{
namespace Helper
{

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

QStringDecoder autodetectCodec(const QByteArrayView buffer)
{
    // First 16 bytes for detecting by BOM.
    const QByteArrayView bufferForBom(buffer.begin(), qMin(16, buffer.size()));

    // first: try to get encoding by BOM handling
    // If BOM has been found, trust it
    if (auto encoding = QStringConverter::encodingForData(bufferForBom)) {
        return QStringDecoder(encoding.value());
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

} // namespace Helper
} // namespace KFileMetaData

#endif // DETECTTEXTCODEC_P_H
