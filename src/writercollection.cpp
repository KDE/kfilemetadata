/*
 * This file is part of the KFileMetaData project
 * Copyright (C) 2016  Varun Joshi <varunj.1011@gmail.com>
 * Copyright (C) 2016  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "writercollection.h"
#include "writer.h"
#include "writer_p.h"
#include "writerplugin.h"
#include "externalwriter.h"
#include "kfilemetadata_debug.h"
#include "config-kfilemetadata.h"

#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <KLocalizedString>

using namespace KFileMetaData;

class Q_DECL_HIDDEN WriterCollection::WriterCollectionPrivate
{
public:
    QHash<QString, Writer*> m_writers;

    QList<Writer*> allWriters() const;
};

WriterCollection::WriterCollection()
    : d_ptr(new WriterCollectionPrivate)
{
    Q_D(WriterCollection);
    const QList<Writer*> all = d->allWriters();

    for (Writer* writer : all) {
        const QStringList lst = writer->mimetypes();
        for (const QString& type : lst) {
            d->m_writers.insertMulti(type, writer);
        }
    }
}

WriterCollection::~WriterCollection()
{
    Q_D(WriterCollection);
    qDeleteAll(d->m_writers.begin(), d->m_writers.end());
    delete d;
}


QList<Writer*> WriterCollection::WriterCollectionPrivate::allWriters() const
{
    QStringList plugins;
    QStringList pluginPaths;
    QStringList externalPlugins;
    QStringList externalPluginPaths;

    const QStringList paths = QCoreApplication::libraryPaths();
    for (const QString& libraryPath : paths) {
        QString path(libraryPath + QStringLiteral("/kf5/kfilemetadata/writers"));
        QDir dir(path);

        if (!dir.exists()) {
            continue;
        }

        const QStringList entryList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        for (const QString& fileName : entryList) {
            // Make sure the same plugin is not loaded twice, even if it
            // installed in two different locations
            if (plugins.contains(fileName))
                continue;

            plugins << fileName;
            pluginPaths << dir.absoluteFilePath(fileName);
        }
    }
    plugins.clear();

    QDir externalPluginDir(QStringLiteral(LIBEXEC_INSTALL_DIR) + QStringLiteral("/kfilemetadata/writers/externalwriters"));
    // For external plugins, we look into the directories
    const QStringList externalPluginEntryList = externalPluginDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& externalPlugin : externalPluginEntryList) {
        if (externalPlugins.contains(externalPlugin))
            continue;

        externalPlugins << externalPlugin;
        externalPluginPaths << externalPluginDir.absoluteFilePath(externalPlugin);
    }
    externalPlugins.clear();

    QList<Writer*> writers;
    for (const QString& pluginPath : qAsConst(pluginPaths)) {
        QPluginLoader loader(pluginPath);

        if (!loader.load()) {
            qCWarning(KFILEMETADATA_LOG) << "Could not create Writer: " << pluginPath;
            qCWarning(KFILEMETADATA_LOG) << loader.errorString();
            continue;
        }

        QObject* obj = loader.instance();
        if (obj) {
            WriterPlugin* plugin = qobject_cast<WriterPlugin*>(obj);
            if (plugin) {
                Writer* writer = new Writer;
                writer->d_ptr->m_plugin = plugin;

                writers << writer;
            } else {
                qCDebug(KFILEMETADATA_LOG) << "Plugin could not be converted to a WriterPlugin";
                qCDebug(KFILEMETADATA_LOG) << pluginPath;
            }
        }
        else {
            qCDebug(KFILEMETADATA_LOG) << "Plugin could not create instance" << pluginPath;
        }
    }

    for (const QString& externalPluginPath : qAsConst(externalPluginPaths)) {
        ExternalWriter *plugin = new ExternalWriter(externalPluginPath);
        Writer* writer = new Writer;
        writer->d_ptr->m_plugin = plugin;

        writers << writer;
    }

    return writers;
}

QList<Writer*> WriterCollection::fetchWriters(const QString& mimetype) const
{
    Q_D(const WriterCollection);
    QList<Writer*> plugins = d->m_writers.values(mimetype);
    if (plugins.isEmpty()) {
        auto it = d->m_writers.constBegin();
        for (; it != d->m_writers.constEnd(); ++it) {
            if (mimetype.startsWith(it.key()))
                plugins << it.value();
        }
    }

    return plugins;
}



