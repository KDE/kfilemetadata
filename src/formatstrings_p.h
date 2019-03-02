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

#ifndef KFILEMETADATA_FORMATSTRINGS_P_H
#define KFILEMETADATA_FORMATSTRINGS_P_H

#include <QString>
#include <QVariant>
#include <KFormat>

namespace KFileMetaData {

class Q_DECL_HIDDEN FormatStrings
{
public:
    static QString toStringFunction(const QVariant& value);

    static QString joinStringListFunction(const QVariant& value);

    static QString formatDate(const QVariant& value);

    static QString formatDuration(const QVariant& value);

    static QString formatBitRate(const QVariant& value);

    static QString formatSampleRate(const QVariant& value);

    static QString formatOrientationValue(const QVariant& value);

    static QString formatPhotoFlashValue(const QVariant& value);

    static QString formatAsDegree(const QVariant& value);

    static QString formatAsMeter(const QVariant& value);

    static QString formatAsFrameRate(const QVariant& value);

    static QString formatAspectRatio(const QVariant& value);

};

}

#endif
