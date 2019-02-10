/*
 * Copyright (C) 2018  Alexander Stippich <a.stippich@gmx.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "formatstrings_p.h"

#include <QDateTime>
#include <KLocalizedString>

using namespace KFileMetaData;

QString FormatStrings::toStringFunction(const QVariant& value)
{
    return value.toString();
}

QString FormatStrings::joinStringListFunction(const QVariant& value)
{
    return value.toStringList().join(i18nc("Separation between multiple entries in a list", ", "));
}

QString FormatStrings::formatDate(const QVariant& value)
{
    KFormat form;
    QDateTime dt;
    if (value.type() == QVariant::DateTime) {
        dt = value.toDateTime();
    } else {
        dt = QDateTime::fromString(value.toString(), Qt::ISODate);
    }
    if (dt.isValid()) {
        return form.formatRelativeDateTime(dt, QLocale::LongFormat);
    }
    return QString();
}

QString FormatStrings::formatDuration(const QVariant& value)
{
    KFormat form;
    return form.formatDuration(value.toInt() * 1000);
}

QString FormatStrings::formatBitRate(const QVariant& value)
{
    KFormat form;
    return i18nc("@label bitrate (per second)", "%1/s", form.formatByteSize(value.toInt(), 0, KFormat::MetricBinaryDialect));
}

QString FormatStrings::formatSampleRate(const QVariant& value)
{
    return i18nc("@label samplerate in kilohertz", "%1 kHz", QLocale().toString(value.toDouble() / 1000));
}

QString FormatStrings::formatOrientationValue(const QVariant& value)
{
    QString string;
    switch (value.toInt()) {
    case 1: string = i18nc("Description of image orientation", "Unchanged"); break;
    case 2: string = i18nc("Description of image orientation", "Horizontally flipped"); break;
    case 3: string = i18nc("Description of image orientation", "180° rotated"); break;
    case 4: string = i18nc("Description of image orientation", "Vertically flipped"); break;
    case 5: string = i18nc("Description of image orientation", "Transposed"); break;
    case 6: string = i18nc("Description of image orientation, counter clock-wise rotated", "90° rotated CCW "); break;
    case 7: string = i18nc("Description of image orientation", "Transversed"); break;
    case 8: string = i18nc("Description of image orientation, counter clock-wise rotated", "270° rotated CCW"); break;
    default:
        break;
    }
    return string;
}

QString FormatStrings::formatAsDegree(const QVariant& value)
{
    return QString(QLocale().toString(value.toDouble()) + i18nc("Symbol of degree, no space", "°"));
}

QString FormatStrings::formatAsMeter(const QVariant& value)
{
    KFormat form;
    return form.formatValue(value.toDouble(), KFormat::Unit::Meter, 1, KFormat::UnitPrefix::AutoAdjust, KFormat::MetricBinaryDialect);
}

QString FormatStrings::formatAsFrameRate(const QVariant& value)
{
    return QString(value.toString() + i18nc("Symbol of frames per second, with space", " fps"));
}
