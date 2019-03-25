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

#ifndef KFILEMETADATA_MIMEUTILS
#define KFILEMETADATA_MIMEUTILS

#include <QMimeDatabase>
#include "kfilemetadata_export.h"

namespace KFileMetaData
{
namespace MimeUtils
{

/**
 * Returns the mimetype for a file
 *
 * The function uses both content and filename to determine the
 * \c QMimeType. In case the extension mimetype is more specific
 * than the content mimetype, and the first inherits the latter,
 * the extension mimetype is preferred.
 * If the extension does not match the content, the content has
 * higher priority.
 * The file must exist and be readable.
 *
 * @since 5.57
 *
 * \sa QMimeDatabase::mimeTypesForFileName
 * \sa QMimeType::inherits
 */
KFILEMETADATA_EXPORT
QMimeType strictMimeType(const QString& filePath, const QMimeDatabase& db);


} // namespace MimeUtils
} // namespace KFileMetaData

#endif // KFILEMETADATA_MIMEUTILS
