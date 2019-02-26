/*
 * Copyright (C) 2012  Vishesh Handa <me@vhanda.in>
 * Copyright (C) 2016  Varun Joshi <varunj.1011@gmail.com>
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

#include "extractor.h"
#include "extractorplugin.h"
#include "extractorcollection.h"
#include "externalextractor.h"
#include "kfilemetadata_debug.h"

#include <QMimeDatabase>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>

#include "config-kfilemetadata.h"

using namespace KFileMetaData;

class Q_DECL_HIDDEN ExtractorCollection::Private
{
public:
    QHash<QString, Extractor*> m_extractors;

    QList<Extractor*> m_allExtractors;

    QList<Extractor*> allExtractors() const;
};

ExtractorCollection::ExtractorCollection()
    : d(new Private)
{
    d->m_allExtractors = d->allExtractors();

    for (Extractor* ex : qAsConst(d->m_allExtractors)) {
        const QStringList lstMimetypes = ex->mimetypes();
        for (const QString& type : lstMimetypes) {
            d->m_extractors.insertMulti(type, ex);
        }
    }
}

ExtractorCollection::~ExtractorCollection()
{
    qDeleteAll(d->m_allExtractors.begin(), d->m_allExtractors.end());
    delete d;
}


QList<Extractor*> ExtractorCollection::allExtractors() const
{
    return d->m_allExtractors;
}

QList<Extractor*> ExtractorCollection::Private::allExtractors() const
{
    QStringList plugins;
    QStringList pluginPaths;
    QStringList externalPlugins;
    QStringList externalPluginPaths;

    const QStringList paths = QCoreApplication::libraryPaths();
    for (const QString& libraryPath : paths) {
        QString path(libraryPath + QStringLiteral("/kf5/kfilemetadata"));
        QDir dir(path);

        if (!dir.exists()) {
            continue;
        }

        const QStringList entryList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString& fileName : entryList) {
            // Make sure the same plugin is not loaded twice, even if it is
            // installed in two different locations
            if (plugins.contains(fileName))
                continue;

            plugins << fileName;
            pluginPaths << dir.absoluteFilePath(fileName);
        }
    }
    plugins.clear();

    QDir externalPluginDir(QStringLiteral(LIBEXEC_INSTALL_DIR) + QStringLiteral("/kfilemetadata/externalextractors"));
    // For external plugins, we look into the directories
    const QStringList externalPluginEntryList = externalPluginDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& externalPlugin : externalPluginEntryList) {
        if (externalPlugins.contains(externalPlugin))
            continue;

        externalPlugins << externalPlugin;
        externalPluginPaths << externalPluginDir.absoluteFilePath(externalPlugin);
    }
    externalPlugins.clear();

    QList<Extractor*> extractors;
    for (const QString& pluginPath : qAsConst(pluginPaths)) {
        QPluginLoader loader(pluginPath);

        if (!loader.load()) {
            qWarning() << "Could not create Extractor: " << pluginPath;
            qWarning() << loader.errorString();
            continue;
        }

        QObject* obj = loader.instance();
        if (obj) {
            ExtractorPlugin* plugin = qobject_cast<ExtractorPlugin*>(obj);
            if (plugin) {
                Extractor* ex= new Extractor;
                ex->setExtractorPlugin(plugin);
                ex->setAutoDeletePlugin(Extractor::DoNotDeletePlugin);
                auto metadata = loader.metaData().value(QLatin1String("MetaData"));
                if (metadata.type() == QJsonValue::Object) {
                    qCDebug(KFILEMETADATA_LOG) << "Created Plugin with metadata:" << metadata;
                    ex->setMetaData(metadata.toObject().toVariantMap());
                }

                extractors << ex;
            } else {
                qCDebug(KFILEMETADATA_LOG) << "Plugin could not be converted to an ExtractorPlugin";
                qCDebug(KFILEMETADATA_LOG) << pluginPath;
            }
        }
        else {
            qCDebug(KFILEMETADATA_LOG) << "Plugin could not create instance" << pluginPath;
        }
    }

    for (const QString& externalPluginPath : qAsConst(externalPluginPaths)) {
        ExternalExtractor *plugin = new ExternalExtractor(externalPluginPath);
        Extractor* extractor = new Extractor;
        extractor->setExtractorPlugin(plugin);
        extractor->setAutoDeletePlugin(Extractor::AutoDeletePlugin);

        extractors << extractor;
    }

    return extractors;
}

QList<Extractor*> ExtractorCollection::fetchExtractors(const QString& mimetype) const
{
    QList<Extractor*> plugins = d->m_extractors.values(mimetype);
    if (!plugins.isEmpty()) {
        return plugins;
    }

    // try to find the best matching more generic extractor by mimetype inheritance
    QMimeDatabase db;
    auto type = db.mimeTypeForName(mimetype);
    const QStringList ancestors = type.allAncestors();

    for (auto ancestor : ancestors) {
        QList<Extractor*> plugins = d->m_extractors.values(ancestor);
        if (!plugins.isEmpty()) {
            qCDebug(KFILEMETADATA_LOG) << "Using inherited mimetype" << ancestor <<  "for" << mimetype;
            return plugins;
        }
    }

    return plugins;
}
