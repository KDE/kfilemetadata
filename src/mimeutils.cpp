/*
 * This file is part of KFileMetaData
 * Copyright (C) 2019 Stefan Brüns <stefan.bruens@rwth-aachen.de>
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

#include "mimeutils.h"

namespace KFileMetaData {
namespace MimeUtils {

QMimeType strictMimeType(const QString& filePath, const QMimeDatabase& db)
{
    auto extensionMimes = db.mimeTypesForFileName(filePath);
    auto contentMime    = db.mimeTypeForFile(filePath, QMimeDatabase::MatchContent);

    if (extensionMimes.contains(contentMime)) {
        // content based mime type is one of the types for the file extension, e.g.:
        // *.ogg -> [ audio/ogg, audio/x-vorbis+ogg, ...]
        // content -> audio/x-vorbis+ogg
        return contentMime;
    }

    for (const auto &mime : extensionMimes) {
        // check if the content is generic and the extension is more specific, e.g.:
        // *.mkv -> [ video/matroska ]
        // content -> application/matroska
        if (mime.inherits(contentMime.name())) {
            return mime;
        }
    }
    // content mime type does not match the extension, trust the content
    return contentMime;
}

}} // namespace KFileMetaData::MimeUtils
