/*
    SPDX-FileCopyrightText: 2013 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#ifndef ODF_EXTRACTOR_H
#define ODF_EXTRACTOR_H

#include "extractorplugin.h"

namespace KFileMetaData
{

class OdfExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin"
                      FILE "odfextractor.json")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit OdfExtractor(QObject* parent = nullptr);

    QStringList mimetypes() const override;
    void extract(ExtractionResult* result) override;

private:
};
}

#endif // ODF_EXTRACTOR_H
