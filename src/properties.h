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

/**
 * @brief The Property enum contains all files property types that KFileMetaData manipulates
 *
 * @todo KF6 remove PropertyCount and LastProperty such that one can easily add new properties
 */
enum Property {
    FirstProperty = 0,
    Empty = 0,

    /**
     * The Bit Rate of the Audio in the File. Represented as an integer
     * in kbit/sec
     */
    BitRate,

    /**
     * The number of channels of the Audio in the File. Represented as an
     * integer.
     */
    Channels,

    /**
     * The duration of the media in the file. Represented as an integer
     * in seconds.
     */
    Duration,

    /**
     * The Genre of an Audio file. This s represented as a string
     * of genres and not integers. The IDv1 standard provides a list of
     * commonly excepted genres.
     */
    Genre,

    /**
     * The same rate or frequency of the Audio in the file. This is represented
     * as an integer in Hz. So a file with "44.1KHz" will have a frequency
     * of 44100
     */
    SampleRate,

    /**
     * Represnts the track number in a set. Typically maps to the "TRCK" tag
     * in IDv3
     */
    TrackNumber,

    /**
     * Indicates the year a track was released. Represented as an integer.
     * Typically mapped to the "TYE (Year)" tag in IDv1
     */
    ReleaseYear,

    /**
     * Represents the Comment or Description stored in the file. This can map
     * to the 'dc:description' tag from DublinCore or the "COMM" field from IDv3
     */
    Comment,

    /**
     * Represents the artist of a media file. This generally corresponds
     * to the IDv1 ARTIST tag. Many extractors often split this string
     * into a number of artists.
     */
    Artist,

    /**
     * Represnts the album of a media file. This generally corresponds
     * to the IDv1 ALBUM tag.
     */
    Album,

    /**
     * Represnts the album artist of a media file. This generally corresponds
     * to the IDv3 TPE2 ("Band/Orchestra/Accompaniment") tag.
     */
    AlbumArtist,

    /**
     * Represnts the Composer of a media file. This generally corresponds
     * to the IDv2 COMPOSER tag.
     */
    Composer,

    /**
     * Represnts the Lyricist of a media file. This generally corresponds
     * to the IDv2 "Lyricist/text writer" tag.
     */
    Lyricist,

    /**
     * The Author field indicated the primary creator of a document.
     * This often corresponds directly to dc:creator
     */
    Author,

    /**
     * Refers to the Title of the content of the file. This can represented
     * by the IDv1 tag TT2 (Title/songname/content description) or the TITLE
     * in a PDF file or the 'dc:title' tag in DublinCore.
     */
    Title,

    /**
     * Refers to the subject of the file. This directly corresponds to the
     * 'dc:subject' tag from DublinCore.
     */
    Subject,

    /**
     * Refers to the Application used to create this file. In the ODF standard
     * this maps to the 'meta:generator' tag. In PDFs its mapped to the
     * "Producer" tag.
     */
    Generator,

    /**
     * The number of pages in a document
     */
    PageCount,

    /**
     * The number of words in a document. This is often only provided for
     * documents where the word count is available in the metadata.
     */
    WordCount,

    /**
     * The number of lines in a document. This is often only provided for
     * documents where the line count is available in the metadata.
     */
    LineCount,

    /**
     * The language the document is written in. This directly maps to the
     * 'dc:language' tag from DublinCore. We do NOT employ any language
     * detection schemes on the text.
     */
    //KF6 TODO: fix typo Langauge->Language
    Langauge,

    /**
     * The copyright of the file. Represented as a string.
     */
    Copyright,

    /**
     * The publisher of the content. Represented as a string.
     */
    Publisher,

    /**
     * The date the content of the file was created. This is extracted
     * from the File MetaData and not from the file system.
     * In ODF, it corresponds to "meta:creation-date", in PDF to the
     * "CreationDate" tag, and otherwise the "dcterms:created" tag.
     */
    CreationDate,

    /**
     * The keywords used to represent the document. This is mostly a string list
     * of all the keywords.
     */
    Keywords,

    /**
     * Represents the width of the Media in pixels. This is generally
     * only applicable for Images and Videos.
     */
    Width,

    /**
     * Represents the height of the Media in pixels. This is generally
     * only applicable for Images and Videos.
     */
    Height,

    /**
     * The Aspect Ratio of the visual image or video.
     * It is the width of a pixel divided by the height of the pixel.
     */
    AspectRatio,

    /**
     * Number of frames per second
     */
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
    PhotoGpsLatitude,
    PhotoGpsLongitude,
    PhotoGpsAltitude,

    TranslationUnitsTotal,
    TranslationUnitsWithTranslation,
    TranslationUnitsWithDraftTranslation,
    TranslationLastAuthor,
    TranslationLastUpDate,
    TranslationTemplateDate,

    /**
     * The URL this file has originally been downloaded from.
     */
    OriginUrl,

    /**
     * The subject of the email this file was originally attached to.
     */
    OriginEmailSubject,

    /**
     * The sender of the email this file was originally attached to.
     */
    OriginEmailSender,

    /**
     * The message ID of the email this file was originally attached to.
     */
    OriginEmailMessageId,

    /**
     * Represents the disc number in a multi-disc set. Typically maps to the "TPOS" tag for mp3
     */
    DiscNumber,

    /**
     * Represents the location where an audio file was recorded.
     */
    Location,

    /**
     * Represents the (lead) performer of an audio file.
     */
    Performer,

    /**
     * Represents the ensemble of an audio file.
     */
    Ensemble,

    /**
     * Represents the arranger of an audio file.
     */
    Arranger,

    /**
     * Represents the conductor of an audio file.
     */
    Conductor,

    /**
     * Represents the opus of an audio file mostly used for classical music.
     */
    Opus,

    /**
     * Represents the label of the content.
     */
    Label,

    /**
     * Containes the name of the compilation of an audio file.
     */
    Compilation,

    /**
     * Contains the license information of the file
     */
    License,

    PropertyCount,
    LastProperty = PropertyCount-1,

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
