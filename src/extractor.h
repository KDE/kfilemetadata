/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_EXTRACTOR_H
#define KFILEMETADATA_EXTRACTOR_H

#include "kfilemetadata_export.h"
#include <QStringList>
#include <QVariantMap>

#include <memory>

namespace KFileMetaData {

class ExtractionResult;
class ExtractorCollection;
class ExtractorPlugin;
class ExtractorPrivate;

/**
 * \class Extractor extractor.h <KFileMetaData/Extractor>
 */
class KFILEMETADATA_EXPORT Extractor
{
    enum ExtractorPluginOwnership {
        AutoDeletePlugin,
        DoNotDeletePlugin,
    };

public:
    Extractor(Extractor&&);
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

    friend class ExtractorCollection;
    friend class ExtractorCollectionPrivate;
    friend class ExtractorPrivate;
    std::unique_ptr<ExtractorPrivate> d;
};
}

#endif // KFILEMETADATA_EXTRACTOR_H
