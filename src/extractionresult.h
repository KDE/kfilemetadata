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
#include "properties.h"
#include "types.h"

namespace KFileMetaData {

/**
 * \class ExtractionResult extractionresult.h <KFileMetaData/ExtractionResult>
 *
 * \brief The ExtractionResult class is where all the data extracted by
 * the indexer is saved. This class acts as a base class which should be
 * derived from and then passed to the relevant plugins.
 *
 * The derived class needs to implement 3 pure virtual functions through
 * which it receives the extracted data.
 *
 * \author Vishesh Handa <me@vhanda.in>
 */
class KFILEMETADATA_EXPORT ExtractionResult
{
public:
    enum Flag {
        ExtractNothing = 0,
        ExtractMetaData = 1,
        ExtractPlainText = 2,
        ExtractEverything = (ExtractMetaData | ExtractPlainText)
    };
    Q_DECLARE_FLAGS(Flags, Flag)

    /**
     * Create an ExtractionResult which can be passed be to Extractors. The
     * extractors use the \p url, \p mimetype and \p flags in order to determine
     * which file the data should be extracted from and which data should
     * be extracted.
     */
    ExtractionResult(const QString& url, const QString& mimetype = QString(), const Flags& flags = ExtractEverything);
    ExtractionResult(const ExtractionResult& rhs);
    virtual ~ExtractionResult();

    /**
     * The input url which the plugins will use to locate the file
     */
    QString inputUrl() const;

    /**
     * The input mimetype. This mimetype should correspond with the
     * mimetypes supported with the relevant plugin when it is being
     * passed to the Extractor
     */
    QString inputMimetype() const;

    /**
     * The flags which the extraction plugin should considering following
     * when extracting metadata from the file
     */
    Flags inputFlags() const;

    /**
     * This function is called by plugins when they wish for some plain
     * text to be indexed without any property. This generally corresponds
     * to the text content in a file
     */
    virtual void append(const QString& text) = 0;

    /**
     * This function is called by the plugins when they wish to
     * add a key value pair which should be indexed. This function may be
     * called multiple times for the same key.
     *
     * \p property This specifies a property name. It should be one of the
     *             properties from the global list of properties.
     *
     * \p value The value of the property
     */
    virtual void add(Property::Property property, const QVariant& value) = 0;

    /**
     * This function is called by the plugins.
     * A type is a higher level classification of the file. A file can
     * have multiple types, but mostly when it does, those types are related.
     * Eg - Document and Presenentation.
     *
     * Please choose one type from the list of available types
     */
    virtual void addType(Type::Type type) = 0;

private:
    class Private;
    Private* d;
};

}

Q_DECLARE_OPERATORS_FOR_FLAGS(KFileMetaData::ExtractionResult::Flags)

#endif // _KFILEMETADATA_EXTRACTIONRESULT_H
