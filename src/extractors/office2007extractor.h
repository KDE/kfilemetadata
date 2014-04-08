/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2013  Vishesh Handa <me@vhanda.in>

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


#ifndef OFFICE_2007_EXTRACTOR_H
#define OFFICE_2007_EXTRACTOR_H

#include "extractorplugin.h"

class KArchiveDirectory;

namespace KFileMetaData
{

class Office2007Extractor : public ExtractorPlugin
{
public:
    Office2007Extractor(QObject* parent, const QVariantList&);

    virtual QStringList mimetypes() const;
    virtual void extract(ExtractionResult* result);

private:
    void extractTextWithTag(QIODevice* device, const QString& tag, ExtractionResult* result);
    void extractAllText(QIODevice* device, ExtractionResult* result);
    void extractTextFromFiles(const KArchiveDirectory* archiveDir, ExtractionResult* result);
};
}

#endif // OFFICE_2007_EXTRACTOR_H
