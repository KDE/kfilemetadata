/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>

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


#ifndef EXIV2EXTRACTOR_H
#define EXIV2EXTRACTOR_H

#include "extractorplugin.h"
#include <exiv2/exiv2.hpp>

namespace KFileMetaData
{

class Exiv2Extractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit Exiv2Extractor(QObject* parent = nullptr);

    void extract(ExtractionResult* result) Q_DECL_OVERRIDE;
    QStringList mimetypes() const Q_DECL_OVERRIDE;

private:
    void add(ExtractionResult* result, const Exiv2::ExifData& data,
             Property::Property prop,
             const char* name, QVariant::Type type);

    double fetchGpsDouble(const Exiv2::ExifData& data, const char* name);
    QByteArray fetchByteArray(const Exiv2::ExifData& data, const char* name);
};
}

#endif // EXIV2EXTRACTOR_H
