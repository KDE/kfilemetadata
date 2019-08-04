/*
 * EmbeddedImageData extracts binary data of cover art files.
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

#ifndef KFILEMETADATA_EMBEDDEDIMAGEDATA_H
#define KFILEMETADATA_EMBEDDEDIMAGEDATA_H

#include "kfilemetadata_export.h"
#include <QByteArray>
#include <QFlags>
#include <QMap>
#include <memory>

namespace KFileMetaData {

/**
 * \class EmbeddedImageData embeddedimagedata.h <KFileMetaData/EmbeddedImageData>
 *
 * \brief The EmbeddedImageData is a class which extracts and writes the images
 * stored in the metadata tags of files as byte arrays. For example, the front
 * cover art of music albums may be extracted with this class. The byte array
 * will mostly contain jpeg or png files, which can be loaded into e.g. QImage.
 *
 * \author Alexander Stippich <a.stippich@gmx.net>
 */
class KFILEMETADATA_EXPORT EmbeddedImageData {
public:
    EmbeddedImageData();
    virtual ~EmbeddedImageData();
    enum ImageType {
        FrontCover = 0x01,
        AllImages = FrontCover
    };
    Q_DECLARE_FLAGS(ImageTypes, ImageType)

    /**
     * Extracts the images stored in the metadata tags from a file.
     * By default, the front cover is extracted.
     */
    QMap<ImageType, QByteArray> imageData(const QString &fileUrl, const EmbeddedImageData::ImageTypes types = FrontCover) const;

    /**
     * Provides a list of mimetypes which are supported for reading
     * of embedded images in e.g. audio files.
     * @since 5.52
     */
    QStringList mimeTypes() const;

    /**
     * Write the images to the metadata tags in a file.
     * Currently, only the front cover is supported.
     * Mimetype is only correctly set for jpeg and png files.
     * @since 5.62
     */
    void writeImageData(const QString &fileUrl, QMap<ImageType, QByteArray> &imageData);

private:
    class Private;
    std::unique_ptr<Private> d;
    EmbeddedImageData& operator=(const EmbeddedImageData&);
};

}

#endif // KFILEMETADATA_EMBEDDEDIMAGEDATA_H
