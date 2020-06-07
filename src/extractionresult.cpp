/*
    SPDX-FileCopyrightText: 2013 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "extractionresult.h"

#include <QMimeDatabase>

using namespace KFileMetaData;

class Q_DECL_HIDDEN ExtractionResult::Private
{
public:
    QString url;
    QString mimetype;
    ExtractionResult::Flags flags;
};

ExtractionResult::ExtractionResult(const QString& url, const QString& mimetype, const Flags& flags)
    : d(new Private)
{
    d->url = url;
    d->mimetype = mimetype;
    if (mimetype.isEmpty()) {
        d->mimetype = QMimeDatabase().mimeTypeForFile(url).name();
    }
    d->flags = flags;
}

ExtractionResult::ExtractionResult(const ExtractionResult& rhs)
    : d(new Private(*rhs.d))
{
}

ExtractionResult::~ExtractionResult()
{
    delete d;
}

QString ExtractionResult::inputUrl() const
{
    return d->url;
}

QString ExtractionResult::inputMimetype() const
{
    return d->mimetype;
}

ExtractionResult::Flags ExtractionResult::inputFlags() const
{
    return d->flags;
}

