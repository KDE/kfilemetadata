/*
 * Copyright (C) 2013  Vishesh Handa <me@vhanda.in>
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

#ifndef _KFILEMETADATA_EXTRACTIONRESULT_H
#define _KFILEMETADATA_EXTRACTIONRESULT_H

#include <QString>
#include <QVariant>

#include "kfilemetadata_export.h"

namespace KFileMetaData {

/**
 * \class ExtractionResult extractionresult.h
 *
 * \brief The ExtractionResult class is where all the data extracted by
 * the indexer is saved. This class acts as a base class which should be
 * dervied from and then passed to the relevant plugins.
 *
 * \author Vishesh Handa <me@vhanda.in>
 */
class KFILEMETADATA_EXPORT ExtractionResult
{
public:
    ExtractionResult();
    ExtractionResult(const ExtractionResult& rhs);
    virtual ~ExtractionResult();

    /**
     * The input url which the plugins will use to locate the file
     */
    QString inputUrl() const;
    void setInputUrl(const QString& url);

    /*
     * The input mimetype. This mimetype should correspond with the
     * mimetypes supported with the relevant plugin when it is being
     * passed to the Extractor
     */
    QString inputMimetype() const;
    void setInputMimetype(const QString& mime);

    /**
     * Add some plain text which should be indexed without any property.
     * This generally corresponds to the text content in a file
     */
    virtual void append(const QString& text) = 0;

    /**
     * Add a key value pair which should be indexed. This function may be
     * called multiple times for the same key.
     *
     * \p key This specifies a property name. It should be of the
     *        form - "artist", "album", "author", etc. Please take a look at
     *        the other plugins to see what properties they are using.
     *
     * \p value The value of the property
     */
    virtual void add(const QString& key, const QVariant& value) = 0;

    /**
     * A type is a higher level classification of the file. Any file can
     * have multiple types. Eg - "Audio" and "Music" or "Video", "Movie"
     */
    virtual void addType(const QString& type) = 0;

private:
    class Private;
    Private* d;
};

}

#endif // _KFILEMETADATA_EXTRACTIONRESULT_H
