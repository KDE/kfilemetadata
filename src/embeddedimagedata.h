/*
    EmbeddedImageData extracts binary data of cover art files.
    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_EMBEDDEDIMAGEDATA_H
#define KFILEMETADATA_EMBEDDEDIMAGEDATA_H

#include "kfilemetadata_export.h"
#include <QByteArray>
#include <QFlags>
#include <QMap>
#include <QMetaType>
#include <memory>

namespace KFileMetaData {

// TODO KF6 make this an enum only in KF6 similar to properties.h
/*!
 * \class KFileMetaData::EmbeddedImageData
 * \inheaderfile KFileMetaData/EmbeddedImageData
 * \inmodule KFileMetaData
 *
 * \brief EmbeddedImageData defines enums for different image types that can
 * be extracted from the metadata of e.g. music files.
 */
class KFILEMETADATA_EXPORT EmbeddedImageData {
public:
    /*!
     *
     */
    EmbeddedImageData();
    virtual ~EmbeddedImageData();
    /*!
     * \value FrontCover
     * \value Other
     * \value FileIcon
     * \value OtherFileIcon
     * \value BackCover
     * \value LeafletPage
     * \value Media
     * \value LeadArtist
     * \value Artist
     * \value Conductor
     * \value Band
     * \value Composer
     * \value Lyricist
     * \value RecordingLocation
     * \value DuringRecording
     * \value DuringPerformance
     * \value MovieScreenCapture
     * \value ColouredFish
     * \value Illustration
     * \value BandLogo
     * \value PublisherLogo
     * \value Unknown
     * \value AllImages
     */
    enum ImageType {
        FrontCover = 1 << 0x0,
        Other = 1 << 0x01,
        FileIcon = 1 << 0x02,
        OtherFileIcon = 1 << 0x03,
        BackCover = 1 << 0x04,
        LeafletPage = 1 << 0x05,
        Media = 1 << 0x06,
        LeadArtist = 1 << 0x07,
        Artist = 1 << 0x08,
        Conductor = 1 << 0x09,
        Band = 1 << 0x0A,
        Composer = 1 << 0x0B,
        Lyricist = 1 << 0x0C,
        RecordingLocation = 1 << 0x0D,
        DuringRecording = 1 << 0x0E,
        DuringPerformance = 1 << 0x0F,
        MovieScreenCapture = 1 << 0x10,
        ColouredFish = 1 << 0x11,
        Illustration = 1 << 0x12,
        BandLogo = 1 << 0x13,
        PublisherLogo = 1 << 0x14,
        Unknown = 1 << 30,
        AllImages = 0x7fffffff
    };
    Q_DECLARE_FLAGS(ImageTypes, ImageType)

private:
    class Private;
    std::unique_ptr<Private> d;
    EmbeddedImageData& operator=(const EmbeddedImageData&) = delete;
};

}

Q_DECLARE_METATYPE(KFileMetaData::EmbeddedImageData::ImageType)
Q_DECLARE_METATYPE(KFileMetaData::EmbeddedImageData::ImageTypes)

#endif // KFILEMETADATA_EMBEDDEDIMAGEDATA_H
