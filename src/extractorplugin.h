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


#ifndef _KFILEMETADATA_EXTRACTOR_H
#define _KFILEMETADATA_EXTRACTOR_H

#include <QStringList>
#include <QDateTime>

#include "kfilemetadata_export.h"
#include "extractionresult.h"

namespace KFileMetaData
{

/**
 * \class ExtractorPlugin extractorplugin.h
 *
 * \brief The ExtractorPlugin is the base class for all file metadata
 * extractors. It is responsible for extracting the metadata in a file.
 *
 * Plugins should derive from this class and implement the mimetypes
 * and extract method.
 *
 * All Plugins should be synchronous and blocking.
 *
 * \author Vishesh Handa <me@vhanda.in>
 */
class KFILEMETADATA_EXPORT ExtractorPlugin : public QObject
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
     * This can also contains partial mimetypes like "text/", in that case
     * this plugin will be chosen only if a better plugin does not exist.
     *
     * \return A StringList containing the mimetypes.
     * \sa extract
     */
    virtual QStringList mimetypes() const = 0;

    /**
     * The main function of the plugin that is responsible for extracting
     * the data and filling up the ExtractionResult
     *
     * The \p result provides the input url and mimetype which
     * can be used to identify the file.
     *
     * This function is synchronous and should be reentrant as it
     * can be called by multiple threads.
     */
    virtual void extract(ExtractionResult* result) = 0;

    //
    // Helper functions
    //

    /**
     * Tries to extract a valid date time from the string provided.
     */
    static QDateTime dateTimeFromString(const QString& dateString);

    /**
     * Tries to split the string into names. It cleans up any superflous words
     * and removes extra junk such as curly braces
     */
    static QStringList contactsFromString(const QString& string);

private:
    class Private;
    Private* d;
};
}

Q_DECLARE_INTERFACE(KFileMetaData::ExtractorPlugin, "org.kde.kf5.kfilemetadata.ExtractorPlugin")

#endif // _KFILEMETADATA_EXTRACTOR_H
