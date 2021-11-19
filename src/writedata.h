/*
    This file is part of the KFileMetaData project
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2016 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef _KFILEMETADATA_WRITEDATA_H
#define _KFILEMETADATA_WRITEDATA_H

#include "kfilemetadata_export.h"

#include <QString>
#include <QMultiMap>
#include <QVariant>

#include "embeddedimagedata.h"
#include "properties.h"
#include "types.h"

namespace KFileMetaData {

/**
 * \class WriteData writedata.h <KFileMetaData/WriteData>
 */
class KFILEMETADATA_EXPORT WriteData
{
public:
    WriteData(const QString& url, const QString& mimetype);
    WriteData(const WriteData& rhs);
    virtual ~WriteData();
    WriteData& operator=(const WriteData& rhs);
    bool operator==(const WriteData& rhs) const;

    QString inputUrl() const;

    QString inputMimetype() const;

    void add(Property::Property property, const QVariant& value);
    void addImageData(const QMap<EmbeddedImageData::ImageType, QByteArray>& images);

    /// @since 5.89
    PropertyMultiMap properties() const;

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(5, 89)
    /// @seprecated Since 5.89, use properties() instead
    KFILEMETADATA_DEPRECATED_VERSION(5, 89, "Use properties() instead")
    QMap<Property::Property, QVariant> getAllProperties() const;
#endif

    QMap<EmbeddedImageData::ImageType, QByteArray> imageData() const;

private:
    class WriteDataPrivate;
    WriteDataPrivate* d_ptr;
    Q_DECLARE_PRIVATE(WriteData)
};
}

#endif // _KFILEMETADATA_WRITEDATA_H
