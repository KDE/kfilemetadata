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


#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QtCore/QStringList>
#include <QtCore/QDateTime>

#include "kmetadata_export.h"

#include <KService>
#include <QStringList>

namespace KMetaData
{

/**
 * \class ExtractorPlugin extractorplugin.h
 *
 * \brief The ExtractorPlugin is the base class for all file metadata
 * extractors. It is responsible for extracting the metadata and providing
 * key value pairs
 *
 * \author Vishesh Handa <me@vhanda.in>
 */
class KMETADATA_EXPORT ExtractorPlugin : public QObject
{
    Q_OBJECT
public:
    ExtractorPlugin(QObject* parent);
    virtual ~ExtractorPlugin();

    /**
     * Provide a list of mimetypes which are supported by this plugin.
     * Only files with those mimetypes will be provided to the plugin via
     * the extract function.
     *
     * \return A StringList containing the mimetypes.
     * \sa extract
     */
    virtual QStringList mimetypes() = 0;

    /**
     * The main function of the plugin that is responsible for extracting the data
     * from the file url and returning a SimpleResourceGraph.
     *
     * It does so on the basis of the mimetype provided.
     *
     * \param resUri The resource uri of the fileUrl which should be used in the SimpleResource
     * \param fileUrl The url of the file being indexed
     * \param mimeType the mimetype of the file url
     */
    virtual QVariantMap extract(const QString& fileUrl, const QString& mimeType) = 0;

    //
    // Helper functions
    //

    /**
     * Tries to extract a valid date time from the string provided.
     */
    static QDateTime dateTimeFromString(const QString& dateString);

    /**
     * Creates a list of nco:Contacts from a list of strings which are separated
     * by a number of different separators. It sets the contact's nco:fullname.
     */
    static QStringList contactsFromString(const QString& string);

private:
};
}

/**
 * Export a file extractor.
 *
 * \param classname The name of the subclass to export
 * \param libname The name of the library which should export the extractor
 */
#define KMETADATA_EXPORT_EXTRACTOR( classname, libname )    \
    K_PLUGIN_FACTORY(factory, registerPlugin<classname>();) \
    K_EXPORT_PLUGIN(factory(#libname))

#endif // EXTRACTOR_H
