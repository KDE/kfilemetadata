/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#ifndef POPPLEREXTRACTOR_H
#define POPPLEREXTRACTOR_H

#include "extractorplugin.h"

namespace KFileMetaData
{

class PopplerExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID kfilemetadata_extractor_iid
                      FILE "popplerextractor.json")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit PopplerExtractor(QObject* parent = nullptr);

    QStringList mimetypes() const override;
    void extract(ExtractionResult* result) override;
};
}

#endif // POPPLEREXTRACTOR_H
