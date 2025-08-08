/*
    SPDX-FileCopyrightText: 2025 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#ifndef CSVEXTRACTOR_H
#define CSVEXTRACTOR_H

#include "extractorplugin.h"

namespace KFileMetaData
{

class CsvExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID kfilemetadata_extractor_iid
                      FILE "csvextractor.json")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit CsvExtractor(QObject* parent = nullptr);

    QStringList mimetypes() const override;
    void extract(ExtractionResult* result) override;
};

}

#endif // CSVEXTRACTOR_H
