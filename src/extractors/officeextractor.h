/*
    This file is part of a KMetaData File Extractor
    Copyright (C) 2013  Denis Steckelmacher <steckdenis@yahoo.fr>

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


#ifndef OFFICE_EXTRACTOR_H
#define OFFICE_EXTRACTOR_H

#include "extractorplugin.h"

namespace KFileMetaData
{

class OfficeExtractor : public ExtractorPlugin
{
public:
    OfficeExtractor(QObject* parent, const QVariantList&);

    virtual QStringList mimetypes() const;
    virtual void extract(ExtractionResult* result);

private:
    void findExe(const QString& mimeType, const QString& name, QString& fullPath);
    QString textFromFile(const QString& fileUrl, const QString& command, QStringList& arguments);

private:
    QStringList m_available_mime_types;

    QString m_catdoc;
    QString m_catppt;
    QString m_xls2csv;
};
}

#endif
