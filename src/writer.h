/*
    This file is part of the KFileMetaData project
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2016 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
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
