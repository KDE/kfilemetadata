/*
 * This file is part of KFileMetaData
 * Copyright (C) 2014 Vishesh Handa <me@vhanda.in>
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

#ifndef KFILEMETADATA_PROPERTIES
#define KFILEMETADATA_PROPERTIES

#include <QMap>
#include <QVariant>

namespace KFileMetaData {
namespace Property {

enum Property {
    FirstProperty = 0,
    Empty = 0,

    // Audio
    BitRate,
    Channels,
    Duration,
    Genre,
    SampleRate,
    TrackNumber,
    ReleaseYear,

    // Maybe merge this with the description?
    Comment,

    // Music
    Artist,
    Album,
    AlbumArtist,
    Composer,
    Lyricist,

    // Documents
    Author,
    Title,
    Subject,
    Creator,
    Generator, // What's the difference?
    PageCount,
    WordCount,
    LineCount,
    Langauge,
    Copyright,
    Publisher,
    Description,
    CreationDate,
    Keywords,

    Width,
    Height,
    AspectRatio,
    FrameRate,

    // Images
    ImageMake,
    ImageModel,
    ImageDateTime,
    ImageOrientation,
    PhotoFlash,
    PhotoPixelXDimension,
    PhotoPixelYDimension,
    PhotoDateTimeOriginal,
    PhotoFocalLength,
    PhotoFocalLengthIn35mmFilm,
    PhotoExposureTime,
    PhotoFNumber,
    PhotoApertureValue,
    PhotoExposureBiasValue,
    PhotoWhiteBalance,
    PhotoMeteringMode,
    PhotoISOSpeedRatings,
    PhotoSaturation,
    PhotoSharpness,

    LastProperty = PhotoSharpness
};

} // namespace Property

typedef QMap<Property::Property, QVariant> PropertyMap;

inline QVariantMap toVariantMap(const PropertyMap& propMap) {
    QVariantMap varMap;
    PropertyMap::const_iterator it = propMap.constBegin();
    for (; it != propMap.constEnd(); ++it) {
        int p = static_cast<int>(it.key());
        varMap.insertMulti(QString::number(p), it.value());
    }

    return varMap;
}

inline PropertyMap toPropertyMap(const QVariantMap& varMap) {
    PropertyMap propMap;
    QVariantMap::const_iterator it = varMap.constBegin();
    for (; it != varMap.constEnd(); ++it) {
        int p = it.key().toInt();
        propMap.insertMulti(static_cast<Property::Property>(p), it.value());
    }

    return propMap;
}

} // namespace KFileMetaData

#endif
