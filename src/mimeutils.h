/*
    This file is part of KFileMetaData
    SPDX-FileCopyrightText: 2019 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
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
