/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
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

#ifndef KFILEMETADATA_EXTRACTOR_H
#define KFILEMETADATA_EXTRACTOR_H

#include "kfilemetadata_export.h"
#include <QStringList>
#include <QVariantMap>

namespace KFileMetaData {

class ExtractionResult;
class ExtractorCollection;
class ExtractorPlugin;

/**
 * \class Extractor extractor.h <KFileMetaData/Extractor>
 */
class KFILEMETADATA_EXPORT Extractor
{
    class ExtractorPrivate;

    enum ExtractorPluginOwnership {
        AutoDeletePlugin,
        DoNotDeletePlugin,
    };

public:
    virtual ~Extractor() noexcept;

    void extract(ExtractionResult* result);
    QStringList mimetypes() const;
    QVariantMap extractorProperties() const;

private:
    Extractor();
    Extractor(const Extractor&);

    void operator =(const Extractor&);

    void setExtractorPlugin(ExtractorPlugin *extractorPlugin);

    void setAutoDeletePlugin(ExtractorPluginOwnership autoDelete);

    void setMetaData(const QVariantMap &metaData);

    ExtractorPrivate *d;
    friend class ExtractorCollection;
};
}

#endif // KFILEMETADATA_EXTRACTOR_H
