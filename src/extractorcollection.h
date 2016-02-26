/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2012  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef _KFILEMETADATA_EXTRACTORCOLLECTION_H
#define _KFILEMETADATA_EXTRACTORCOLLECTION_H

#include "extractor.h"
#include "kfilemetadata_export.h"

namespace KFileMetaData
{
/**
 * \class ExtractorCollection extractorcollection.h
 *
 * \brief The ExtractorCollection is a helper class which internally
 * loads all the extractor plugins. It can be used to fetch a certain
 * subset of thse pulgins based on a given mimetype.
 *
 * Once the appropriate plugins have been fetched, an ExtractionResult
 * should be created and passed to the plugin's extract function.
 *
 * \author Vishesh Handa <me@vhanda.in>
 */
class KFILEMETADATA_EXPORT ExtractorCollection
{
public:
    explicit ExtractorCollection();
    virtual ~ExtractorCollection();

    /**
     * Fetch the extractors which can be used to extract
     * data for the respective file with the given mimetype.
     *
     * If no match is found then all the plugins whose mimetype list
     * starts with \p mimetype are returned.
     */
    QList<Extractor*> fetchExtractors(const QString& mimetype) const;

private:
    class Private;
    Private* d;
};
}

#endif
