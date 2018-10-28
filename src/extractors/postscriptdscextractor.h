/*
    Copyright (C) 2018  Stefan Brüns <stefan.bruens@rwth-aachen.de>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef DSC_EXTRACTOR_H
#define DSC_EXTRACTOR_H

#include "extractorplugin.h"

namespace KFileMetaData
{

class DscExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit DscExtractor(QObject* parent = nullptr);

    QStringList mimetypes() const override;
    void extract(ExtractionResult* result) override;

private:
};

}

#endif // DSC_EXTRACTOR_H
