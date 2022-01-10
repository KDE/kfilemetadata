/*
    This file is part of the KFileMetaData project
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2016 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef _KFILEMETADTA_WRITERCOLLECTION_H
#define _KFILEMETADTA_WRITERCOLLECTION_H

#include <QList>

#include <memory>

#include "kfilemetadata_export.h"
#include "writer.h"

namespace KFileMetaData
{
class WriterCollectionPrivate;
/**
 * \class WriterCollection writercollection.h <KFileMetaData/WriterCollection>
 */
class KFILEMETADATA_EXPORT WriterCollection
{
public:
    explicit WriterCollection();
    virtual ~WriterCollection();

    QList<Writer*> fetchWriters(const QString& mimetype) const;

private:
    const std::unique_ptr<WriterCollectionPrivate> d;
};
}

#endif // _KFILEMETADTA_WRITERCOLLECTION_H
