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

#include <QMimeDatabase>

#include "writedata.h"

using namespace KFileMetaData;

class WriteData::WriteDataPrivate {
public:
    QString url;
    QString mimetype;
    PropertyMap properties;
};

WriteData::WriteData(const QString& url, const QString& mimetype)
    : d_ptr(new WriteDataPrivate)
{
    Q_D(WriteData);

    d->url = url;
    d->mimetype = mimetype;
    if (mimetype.isEmpty()) {
        d->mimetype = QMimeDatabase().mimeTypeForFile(url).name();
    }
}

WriteData::WriteData(const WriteData& rhs)
    : d_ptr(new WriteDataPrivate(*rhs.d_ptr))
{
}

WriteData& WriteData::operator=(const WriteData& rhs)
{
    *d_ptr = *rhs.d_ptr;
    return *this;
}

bool WriteData::operator==(const WriteData& rhs) const
{
    Q_D(const WriteData);
    return d->properties == rhs.d_ptr->properties;
}

void WriteData::add(Property::Property property, const QVariant& value)
{
    Q_D(WriteData);
    d->properties.insertMulti(property, value);
}

WriteData::~WriteData()
{
    delete d_ptr;
}

QString WriteData::inputUrl() const
{
    Q_D(const WriteData);
    return d->url;
}

QString WriteData::inputMimetype() const
{
    Q_D(const WriteData);
    return d->mimetype;
}

PropertyMap WriteData::getAllProperties() const
{
    Q_D(const WriteData);
    return d->properties;
}

