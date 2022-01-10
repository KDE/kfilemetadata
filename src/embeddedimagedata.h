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

/**
 * \class EmbeddedImageData embeddedimagedata.h <KFileMetaData/EmbeddedImageData>
 *
 * \brief EmbeddedImageData defines enums for different image types that can
 * be extracted from the metadata of e.g. music files
 * @todo KF6 make this an enum only in KF6 similar to properties.h
 */
class KFILEMETADATA_EXPORT EmbeddedImageData {
public:
    EmbeddedImageData();
    virtual ~EmbeddedImageData();
    /**
     * @see ImageTypes
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
    /**
     * Stores a combination of #ImageType values.
     */
    Q_DECLARE_FLAGS(ImageTypes, ImageType)

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(5, 91)

    /**
     * Extracts the images stored in the metadata tags from a file.
     * By default, the front cover is extracted.
     * @deprecated Since 5.91 Use ExtractorCollection and ExtractionResult::imageData() instead.
     */
KFILEMETADATA_DEPRECATED_VERSION(5, 91, "Use ExtractorCollection and ExtractionResult::imageData() instead")
    QMap<ImageType, QByteArray> imageData(const QString &fileUrl, const EmbeddedImageData::ImageTypes types = FrontCover) const;

    /**
     * Provides a list of mimetypes which are supported for reading
     * of embedded images in e.g. audio files.
     * @since 5.52
     * @deprecated Since 5.91 Mimetypes are automatically handled when using the Extractor/WriterCollection.
     */
KFILEMETADATA_DEPRECATED_VERSION(5, 91, "Mimetypes are automatically handled when using the Extractor/WriterCollection")
    QStringList mimeTypes() const;
    
    /**
     * Extracts the images stored in the metadata tags from a file.
     * By default, the front cover is extracted.
     * @deprecated Since 5.91 Use WriterCollection and WriteData::addImageData() instead.
     */
KFILEMETADATA_DEPRECATED_VERSION(5, 91, "Use WriterCollection and WriteData::addImageData() instead")
    void writeImageData(const QString &fileUrl, QMap<ImageType, QByteArray> &imageData);
#endif

private:
    class Private;
    std::unique_ptr<Private> d;
    EmbeddedImageData& operator=(const EmbeddedImageData&);
};

}

Q_DECLARE_METATYPE(KFileMetaData::EmbeddedImageData::ImageType)
Q_DECLARE_METATYPE(KFileMetaData::EmbeddedImageData::ImageTypes)

#endif // KFILEMETADATA_EMBEDDEDIMAGEDATA_H
