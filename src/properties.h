/*
    This file is part of KFileMetaData
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_PROPERTIES
#define KFILEMETADATA_PROPERTIES

#include "kfilemetadata_export.h"

#include <QMap>
#include <QVariant>

namespace KFileMetaData {

/*!
 * \namespace KFileMetaData::Property
 * \inmodule KFileMetaData
 * \brief The Property namespace.
 */
namespace Property {

/*!
 * The Property enum contains all files property types that KFileMetaData manipulates
 *
 * \value Empty
 * \value BitRate The Bit Rate of the Audio in the file. Represented as an integer in kbit/sec
 * \value Channels The number of channels of the Audio in the file. Represented as an integer
 * \value Duration The duration of the media in the file. Represented as an integer in seconds
 * \value Genre The Genre of an Audio file. This is represented as a string of genres and not integers. The IDv1 standard provides a list of commonly excepted genres.
 * \value SampleRate The sample rate or frequency of the Audio in the file. This is represented as an integer in Hz. So a file with "44.1KHz" will have a frequency of 44100
 * \value TrackNumber Represents the track number in a set. Typically maps to the "TRCK" tag in IDv3
 * \value ReleaseYear Indicates the year a track was released. Represented as an integer. Typically mapped to the "TYE (Year)" tag in IDv1
 * \value Comment Represents a comment stored in the file. This can map to e.g. the "COMM" field from IDv3
 * \value Artist Represents the artist of a media file. This generally corresponds to the IDv1 ARTIST tag. Many extractors often split this string into a number of artists.
 * \value Album Represents the album of a media file. This generally corresponds to the IDv1 ALBUM tag.
 * \value AlbumArtist Represents the album artist of a media file. This generally corresponds to the IDv3 TPE2 ("Band/Orchestra/Accompaniment") tag.
 * \value Composer Represents the Composer of a media file. This generally corresponds to the IDv2 COMPOSER tag
 * \value Lyricist Represents the Lyricist of a media file. This generally corresponds to the IDv2 "Lyricist/text writer" tag
 * \value Author The Author field indicated the primary creator of a document. This often corresponds directly to dc:creator
 * \value Title Refers to the Title of the content of the file. This can represented by the IDv1 tag TT2 (Title/songname/content description) or the TITLE in a PDF file or the 'dc:title' tag in DublinCore
 * \value Subject Refers to the subject of the file. This directly corresponds to the 'dc:subject' tag from DublinCore.
 * \value Generator Refers to the Application used to create this file. In the ODF standard this maps to the 'meta:generator' tag. In PDFs its mapped to the "Producer" tag.
 * \value PageCount The number of pages in a document
 * \value WordCount The number of words in a document. This is often only provided for documents where the word count is available in the metadata
 * \value LineCount The number of lines in a document. This is often only provided for documents where the line count is available in the metadata
 * \value[since 5.50] Language The language the document is written in. This directly maps to the 'dc:language' tag from DublinCore. We do NOT employ any language detection schemes on the text
 * \value Copyright The copyright of the file. Represented as a string
 * \value Publisher The publisher of the content. Represented as a string
 * \value CreationDate The date the content of the file was created. This is extracted from the file metadata and not from the file system. In ODF, it corresponds to "meta:creation-date", in PDF to the "CreationDate" tag, and otherwise the "dcterms:created" tag
 * \value Keywords The keywords used to represent the document. This is mostly a string list of all the keywords
 * \value Width Represents the width of the Media in pixels. This is generally only applicable for Images and Videos
 * \value Height Represents the height of the Media in pixels. This is generally only applicable for Images and Videos
 * \value AspectRatio The Aspect Ratio of the visual image or video. It is the width of a pixel divided by the height of the pixel
 * \value FrameRate Number of frames per second
 * \value[since 5.60] Manufacturer The manufacturer of the equipment used for generating the file and metadata. Typically maps to the 'Exif.Image.Make' tag
 * \value[since 5.60] Model The model name of the equipment used for generating the file and metadata. Typically maps to the 'Exif.Image.Model' tag
 * \value ImageDateTime
 * \value ImageOrientation
 * \value PhotoFlash
 * \value PhotoPixelXDimension
 * \value PhotoPixelYDimension
 * \value PhotoDateTimeOriginal
 * \value PhotoFocalLength
 * \value PhotoFocalLengthIn35mmFilm
 * \value PhotoExposureTime
 * \value PhotoFNumber
 * \value PhotoApertureValue
 * \value PhotoExposureBiasValue
 * \value PhotoWhiteBalance
 * \value PhotoMeteringMode
 * \value PhotoISOSpeedRatings
 * \value PhotoSaturation
 * \value PhotoSharpness
 * \value PhotoGpsLatitude
 * \value PhotoGpsLongitude
 * \value PhotoGpsAltitude
 * \value TranslationUnitsTotal
 * \value TranslationUnitsWithTranslation
 * \value TranslationUnitsWithDraftTranslation
 * \value TranslationLastAuthor
 * \value TranslationLastUpDate
 * \value TranslationTemplateDate
 * \value OriginUrl The URL this file has originally been downloaded from
 * \value OriginEmailSubject The subject of the email this file was originally attached to
 * \value OriginEmailSender The sender of the email this file was originally attached to
 * \value OriginEmailMessageId The message ID of the email this file was originally attached to
 * \value DiscNumber Represents the disc number in a multi-disc set. Typically maps to the "TPOS" tag for mp3
 * \value Location Represents the location where an audio file was recorded
 * \value Performer Represents the (lead) performer of an audio file
 * \value Ensemble Represents the ensemble of an audio file
 * \value Arranger Represents the arranger of an audio file
 * \value Conductor Represents the conductor of an audio file
 * \value Opus Represents the opus of an audio file mostly used for classical music
 * \value Label Represents the label of the content
 * \value Compilation Contains the name of the compilation of an audio file
 * \value License Contains the license information of the file
 * \value Rating For ratings stored in Metadata tags
 * \value Lyrics Contains the lyrics of a song embedded in the file
 * \value ReplayGainAlbumPeak Contains ReplayGain information for audio files
 * \value ReplayGainAlbumGain Contains ReplayGain information for audio files. The album gain is given in "dB"
 * \value ReplayGainTrackPeak Contains ReplayGain information for audio files
 * \value ReplayGainTrackGain Contains ReplayGain information for audio files. The track gain is given in "dB"
 * \value Description Represents the description stored in the file. This maps to the 'dc:description' tag from DublinCore
 * \value[since 6.13] VideoCodec A string description of the codec used
 * \value[since 6.13] AudioCodec A string description of the codec used
 * \value[since 6.13] PixelFormat The Pixel format used by the media (image or video)
 * \value[since 6.13] ColorSpace The Color space used by the media (image or video)
 */
enum Property {
    Empty = 0,
    BitRate,
    Channels,
    Duration,
    Genre,
    SampleRate,
    TrackNumber,
    ReleaseYear,
    Comment,
    Artist,
    Album,
    AlbumArtist,
    Composer,
    Lyricist,
    Author,
    Title,
    Subject,
    Generator,
    PageCount,
    WordCount,
    LineCount,
    Language,
    Copyright,
    Publisher,
    CreationDate,
    Keywords,
    Width,
    Height,
    AspectRatio,
    FrameRate,
    Manufacturer,
    Model,

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

    OriginUrl,
    OriginEmailSubject,
    OriginEmailSender,
    OriginEmailMessageId,
    DiscNumber,
    Location,
    Performer,
    Ensemble,
    Arranger,
    Conductor,
    Opus,
    Label,
    Compilation,
    License,
    Rating,
    Lyrics,
    ReplayGainAlbumPeak,
    ReplayGainAlbumGain,
    ReplayGainTrackPeak,
    ReplayGainTrackGain,
    Description,

    VideoCodec,
    AudioCodec,
    PixelFormat,
    ColorSpace,
};

} // namespace Property

using PropertyMultiMap = QMultiMap<Property::Property, QVariant>;

} // namespace KFileMetaData

Q_DECLARE_METATYPE(KFileMetaData::Property::Property)
Q_DECLARE_METATYPE(KFileMetaData::PropertyMultiMap)

#endif
