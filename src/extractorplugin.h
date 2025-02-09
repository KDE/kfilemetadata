/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#ifndef _KFILEMETADATA_EXTRACTOR_PLUGIN_H
#define _KFILEMETADATA_EXTRACTOR_PLUGIN_H

#include <QStringList>
#include <QDateTime>

#include "kfilemetadata_export.h"
#include "extractionresult.h"

namespace KFileMetaData
{

/**
 * \class ExtractorPlugin extractorplugin.h <KFileMetaData/ExtractorPlugin>
 *
 * \brief The ExtractorPlugin is the base class for all file metadata
 * extractors.
 *
 * It is responsible for extracting the metadata in a file.
 *
 * Plugins should derive from this class and implement the \c mimetypes()
 * and \c extract() method.
 *
 * The plugin must also specify the implemented interface and provide
 * sufficient metadata:
 *
 * \code{cpp}
 * class FooExtractor : public ExtractorPlugin
 * {
 *     Q_OBJECT
 *     Q_PLUGIN_METADATA(IID kfilemetadata_extractor_iid
 *                       FILE "fooextractor.json")
 *     Q_INTERFACES(KFileMetaData::ExtractorPlugin)
 * ...
 * };
 * \endcode
 * \code{json}
 * {
 *     "Name" : "FooExtractor",
 *     "Id" :   "org.kde.fooextractor",
 *     "MimeTypes" : { "application/x-foo" : { "Version" : "0.0" } }
 * }
 * \endcode
 *
 * \author Vishesh Handa <me@vhanda.in>
 */
class KFILEMETADATA_EXPORT ExtractorPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ExtractorPlugin(QObject* parent);
    ~ExtractorPlugin() override;

    /**
     * Provide a list of MIME types which are supported by this plugin.
     * Only files with those MIME types will be provided to the plugin via
     * the extract function.
     *
     * This can also contains partial MIME types like "text/", in that case
     * this plugin will be chosen only if a better plugin does not exist.
     *
     * \return A StringList containing the MIME types.
     * \sa extract
     */
    virtual QStringList mimetypes() const = 0;

    /**
     * The main function of the plugin that is responsible for extracting
     * the data and filling up the ExtractionResult
     *
     * The \p result provides the input URL and MIME type which
     * can be used to identify the file.
     *
     * This function is synchronous and must be reentrant as it
     * can be called by multiple threads.
     */
    virtual void extract(ExtractionResult* result) = 0;

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(6, 12)
    //
    // Helper functions
    //

    /**
     * Tries to extract a valid date time from the string provided.
     */
    /// @deprecated since 6.12
    static QDateTime dateTimeFromString(const QString& dateString);

    /**
     * Tries to split the string into names. It cleans up any superfluous words
     * and removes extra junk such as curly braces
     */
    /// @deprecated since 6.12
    static QStringList contactsFromString(const QString& string);
#endif

protected:
    /**
     * Return the inherited MIME type which the extractor directly supports.
     *
     * The returned type is one of the types from \c mimetypes(),
     * and is one of the ancestors of the input \p mimetype
     * (including \p mimetype itself).
     *
     * In case the MIME type is not a subtype of the supported types,
     * an empty QString() is returned.
     *
     * \sa ExtractorCollection::fetchExtractors
     * \sa QMimeType::allAncestors
     * @since 5.57
     */
    QString getSupportedMimeType(const QString& mimetype) const;

private:
    class ExtractorPluginPrivate;
    ExtractorPluginPrivate *d_placeholder; // Placeholder for future binary compatible extensions
};
}

#define kfilemetadata_extractor_iid "org.kde.kf5.kfilemetadata.ExtractorPlugin"
Q_DECLARE_INTERFACE(KFileMetaData::ExtractorPlugin, kfilemetadata_extractor_iid)

#endif // _KFILEMETADATA_EXTRACTOR_PLUGIN_H
