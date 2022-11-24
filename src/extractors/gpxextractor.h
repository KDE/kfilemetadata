/*
    SPDX-FileCopyrightText: 2022 Kai Uwe Broulik <kde@broulik.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef GPXEXTRACTOR_H
#define GPXEXTRACTOR_H

#include "extractorplugin.h"

namespace KFileMetaData
{
class GpxExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin" FILE "gpxextractor.json")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit GpxExtractor(QObject *parent = nullptr);

    void extract(ExtractionResult *result) override;
    QStringList mimetypes() const override;
};

}

#endif // GPXEXTRACTOR_H
