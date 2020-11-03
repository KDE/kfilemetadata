/*
    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "embeddedimagedata.h"
#include "extractorcollection.h"
#include "simpleextractionresult.h"
#include "writedata.h"
#include "writercollection.h"
#include "kfilemetadata_debug.h"

#include <QMimeDatabase>

using namespace KFileMetaData;

class Q_DECL_HIDDEN EmbeddedImageData::Private
{
public:
    QMimeDatabase mMimeDatabase;
    static const QStringList mMimetypes;
};

const QStringList EmbeddedImageData::Private::mMimetypes =
{
    QStringLiteral("audio/flac"),
    QStringLiteral("audio/mp4"),
    QStringLiteral("audio/mpeg"),
    QStringLiteral("audio/mpeg3"),
    QStringLiteral("audio/ogg"),
    QStringLiteral("audio/opus"),
    QStringLiteral("audio/wav"),
    QStringLiteral("audio/x-aiff"),
    QStringLiteral("audio/x-ape"),
    QStringLiteral("audio/x-mpeg"),
    QStringLiteral("audio/x-ms-wma"),
    QStringLiteral("audio/x-musepack"),
    QStringLiteral("audio/x-opus+ogg"),
    QStringLiteral("audio/x-speex+ogg"),
    QStringLiteral("audio/x-vorbis+ogg"),
    QStringLiteral("audio/x-wav"),
    QStringLiteral("audio/x-wavpack"),
};

EmbeddedImageData::EmbeddedImageData()
    : d(std::unique_ptr<Private>(new Private()))
{
}

EmbeddedImageData::~EmbeddedImageData()
= default;

QStringList EmbeddedImageData::mimeTypes() const
{
    return d->mMimetypes;
}

QMap<EmbeddedImageData::ImageType, QByteArray>
EmbeddedImageData::imageData(const QString &fileUrl,
                             const EmbeddedImageData::ImageTypes types) const
{
    const auto fileMimeType = d->mMimeDatabase.mimeTypeForFile(fileUrl).name();
    KFileMetaData::ExtractorCollection ec;
    KFileMetaData::SimpleExtractionResult result(fileUrl, fileMimeType, ExtractionResult::ExtractImageData);

    auto extractors = ec.fetchExtractors(fileMimeType);
    for (const auto& ex : extractors) {
	ex->extract(&result);
    }
    return result.imageData();
}

void
EmbeddedImageData::writeImageData(const QString &fileUrl,
                             QMap<EmbeddedImageData::ImageType, QByteArray> &imageData)
{
    const auto fileMimeType = d->mMimeDatabase.mimeTypeForFile(fileUrl).name();
    KFileMetaData::WriterCollection wc;
    KFileMetaData::WriteData data(fileUrl, fileMimeType);
    data.addImageData(imageData);

    auto writers = wc.fetchWriters(fileMimeType);
    for (const auto& w : writers) {
	w->write(data);
    }
}
