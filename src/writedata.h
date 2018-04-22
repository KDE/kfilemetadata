/*
 * This file is part of the KFileMetaData project
 * Copyright (C) 2016  Varun Joshi <varunj.1011@gmail.com>
 * Copyright (C) 2016  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _KFILEMETADATA_WRITEDATA_H
#define _KFILEMETADATA_WRITEDATA_H

#include "kfilemetadata_export.h"

#include <QString>
#include <QMap>
#include <QVariant>

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

    QMap<Property::Property, QVariant> getAllProperties() const;

private:
    class WriteDataPrivate;
    WriteDataPrivate* d_ptr;
    Q_DECLARE_PRIVATE(WriteData)
};
}

#endif // _KFILEMETADATA_WRITEDATA_H
