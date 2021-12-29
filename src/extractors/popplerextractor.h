/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#ifndef POPPLEREXTRACTOR_H
#define POPPLEREXTRACTOR_H

#include "extractorplugin.h"
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <poppler-qt5.h>
#else
#include <poppler-qt6.h>
#endif 

namespace KFileMetaData
{

class PopplerExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin"
                      FILE "popplerextractor.json")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit PopplerExtractor(QObject* parent = nullptr);

    QStringList mimetypes() const override;
    void extract(ExtractionResult* result) override;
};
}

#endif // POPPLEREXTRACTOR_H
