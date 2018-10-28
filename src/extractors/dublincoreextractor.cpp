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

#include "dublincoreextractor.h"
#include "extractionresult.h"

namespace {

inline QString dcNS()      { return QStringLiteral("http://purl.org/dc/elements/1.1/"); }
inline QString dctermsNS() { return QStringLiteral("http://purl.org/dc/terms/"); }

}

namespace KFileMetaData
{

void DublinCoreExtractor::extract(ExtractionResult* result, const QDomNode& fragment)
{
    QDomElement e = fragment.firstChildElement();

    while (!e.isNull()) {
        const QString namespaceURI = e.namespaceURI();
        const QString localName = e.localName();

        // Dublin Core
        // According to http://dublincore.org/documents/dces/, the
        // properties should be treated the same regardless if
        // used in the legacy DCES or DCMI-TERMS variant
        if (namespaceURI == dcNS() || namespaceURI == dctermsNS()) {
            if (localName == QLatin1String("description")) {
                result->add(Property::Comment, e.text());
            } else if (localName == QLatin1String("subject")) {
                result->add(Property::Subject, e.text());
            } else if (localName == QLatin1String("title")) {
                result->add(Property::Title, e.text());
            } else if (localName == QLatin1String("creator")) {
                result->add(Property::Author, e.text());
            } else if (localName == QLatin1String("language")) {
                result->add(Property::Language, e.text());
            }
        }
        e = e.nextSiblingElement();
    }
}

} // namespace KFileMetaData
