/*
    Helper class to extract XML encoded Dublin Core metadata

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


#ifndef DUBLINCORE_EXTRACTOR_H
#define DUBLINCORE_EXTRACTOR_H

#include <QDomNode>

namespace KFileMetaData
{

class ExtractionResult;

class DublinCoreExtractor
{
public:
    /**
      * Extract DC metadata from an XML fragment
      *
      * Prerequsites:
      * - DC element nodes are immediate children to \p fragment
      * - The \c QDomDocument has been parsed with enabled namespaceProcessing
      *
      * \sa QDomDocument::setContent
      */
    static void extract(ExtractionResult* result, const QDomNode& fragment);

};

} // namespace KFileMetaData

#endif // DUBLINCORE_EXTRACTOR_H

