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

#ifndef KFILEMETADTA_WRITER_H
#define KFILEMETADTA_WRITER_H

#include "kfilemetadata_export.h"
#include <QStringList>

namespace KFileMetaData
{

class WriteData;
class WriterCollection;

/**
 * \class Writer writer.h <KFileMetaData/Writer>
 */
class KFILEMETADATA_EXPORT Writer
{
    class WriterPrivate;

    enum WriterPluginOwnership {
        AutoDeletePlugin,
        DoNotDeletePlugin,
    };

public:
    Writer(Writer&&);
    virtual ~Writer();

    void write(const WriteData& data);
    QStringList mimetypes() const;

private:
    Writer();
    Writer(const Writer&);

    void operator =(const Writer&);

    void setAutoDeletePlugin(WriterPluginOwnership autoDelete);

    WriterPrivate *d;
    friend class WriterCollection;
};
}

#endif // KFILEMETADTA_WRITER_H
