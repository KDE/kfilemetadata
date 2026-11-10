/*
    SPDX-FileCopyrightText: 2026 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef LOCATIONPARSER_P_H
#define LOCATIONPARSER_P_H

#include <QRegularExpression>
#include <QDebug>

#include <optional>

using namespace Qt::Literals::StringLiterals;

namespace KFileMetaData
{
namespace Parser
{

std::array<std::optional<float>, 3> locationFromIso6709HString(QStringView location)
{
    std::array<std::optional<float>, 3> result;

    // Format follows ISO 6709 Annex H: ±<latitude>±<longitude>±<elevation><crs_name>/
    // Latitude/longitude is always in integral degrees (i.e. 2 digit/3 digit) format
    // Coordinate Reference System (crs) is omitted, though required
    static const QRegularExpression iso6709rx(uR"(([+-]\d+)(:?[^\d+-](\d+))?)"_s);

    auto matches = iso6709rx.globalMatchView(location);
    int i = 0;
    while (matches.hasNext() && i < 3) {
        const auto match = matches.next();
        bool ok;
        QString valueString = [match] {
            if (match.hasCaptured(3)) {
                // Due to a bug in FFmpeg it may use a locale dependent thousands separator,
                // strip it and replace it with a dot.
                return u"%1.%2"_s.arg(match.capturedView(1), match.capturedView(3));
            }
            return match.captured(1);
        }();
        const auto value = valueString.toFloat(&ok);
        if (ok) {
            result[i] = value;
        }
        ++i;
    }
    return result;
}
} // namespace Helper
} // namespace KFileMetaData

#endif // LOCATIONPARSER_P_H
