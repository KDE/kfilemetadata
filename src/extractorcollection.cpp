/*
 * <one line to give the library's name and an idea of what it does.>
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
#include "extractor_p.h"
#include "externalextractor.h"

#include <QDebug>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>

#include "config-kfilemetadata.h"

using namespace KFileMetaData;

class ExtractorCollection::Private {
public:
    QHash<QString, Extractor*> m_extractors;

    QList<Extractor*> allExtractors() const;
};

ExtractorCollection::ExtractorCollection()
    : d(new Private)
{
    QList<Extractor*> all = d->allExtractors();

    foreach (Extractor* ex, all) {
        foreach (const QString& type, ex->mimetypes()) {
            d->m_extractors.insertMulti(type, ex);
        }
    }
}

ExtractorCollection::~ExtractorCollection()
{
    qDeleteAll(d->m_extractors.values().toSet());
    delete d;
}


QList<Extractor*> ExtractorCollection::Private::allExtractors() const
{
    QStringList plugins;
    QStringList pluginPaths;
    QStringList externalPlugins;
    QStringList externalPluginPaths;

    QStringList paths = QCoreApplication::libraryPaths();
    Q_FOREACH (const QString& libraryPath, paths) {
        QString path(libraryPath + QStringLiteral("/kf5/kfilemetadata"));
        QDir dir(path);

        if (!dir.exists()) {
            continue;
        }

        QStringList entryList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        Q_FOREACH (const QString& fileName, entryList) {
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
    QStringList externalPluginEntryList = externalPluginDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    Q_FOREACH (const QString& externalPlugin, externalPluginEntryList) {
        if (externalPlugins.contains(externalPlugin))
            continue;

        externalPlugins << externalPlugin;
        externalPluginPaths << externalPluginDir.absoluteFilePath(externalPlugin);
    }
    externalPlugins.clear();

    QList<Extractor*> extractors;
    Q_FOREACH (const QString& pluginPath, pluginPaths) {
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
                ex->d->m_plugin = plugin;

                extractors << ex;
            } else {
                qDebug() << "Plugin could not be converted to an ExtractorPlugin";
                qDebug() << pluginPath;
            }
        }
        else {
            qDebug() << "Plugin could not creaate instance" << pluginPath;
        }
    }

    Q_FOREACH (const QString& externalPluginPath, externalPluginPaths) {
        ExternalExtractor *plugin = new ExternalExtractor(externalPluginPath);
        Extractor* extractor = new Extractor;
        extractor->d->m_plugin = plugin;

        extractors << extractor;
    }

    return extractors;
}

QList<Extractor*> ExtractorCollection::fetchExtractors(const QString& mimetype) const
{
    QList<Extractor*> plugins = d->m_extractors.values(mimetype);
    if (plugins.isEmpty()) {
        auto it = d->m_extractors.constBegin();
        for (; it != d->m_extractors.constEnd(); it++) {
            if (mimetype.startsWith(it.key()))
                plugins << it.value();
        }
    }

    return plugins;
}
