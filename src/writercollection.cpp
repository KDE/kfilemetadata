/*
    This file is part of the KFileMetaData project
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2016 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "writercollection.h"
#include "writer_p.h"
#include "writerplugin.h"
#include "externalwriter.h"
#include "kfilemetadata_debug.h"
#include "config-kfilemetadata.h"

#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>
#include <QMimeDatabase>
#include <vector>

using namespace KFileMetaData;

class Q_DECL_HIDDEN WriterCollection::WriterCollectionPrivate
{
public:
    QMultiHash<QString, Writer*> m_writers;

    std::vector<Writer> m_allWriters;

    void findWriters();
};

WriterCollection::WriterCollection()
    : d(new WriterCollectionPrivate)
{
    d->findWriters();
}

WriterCollection::~WriterCollection()
{
    delete d;
}

void WriterCollection::WriterCollectionPrivate::findWriters()
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
            if (!QLibrary::isLibrary(fileName)) {
                continue;
            }
            // Make sure the same plugin is not loaded twice, even if it
            // installed in two different locations
            if (plugins.contains(fileName)) {
                continue;
            }

            plugins << fileName;
            pluginPaths << dir.absoluteFilePath(fileName);
        }
    }
    plugins.clear();

    QDir externalPluginDir(QStringLiteral(LIBEXEC_INSTALL_DIR) + QStringLiteral("/kfilemetadata/writers/externalwriters"));
    // For external plugins, we look into the directories
    const QStringList externalPluginEntryList = externalPluginDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& externalPlugin : externalPluginEntryList) {
        if (!QLibrary::isLibrary(externalPlugin)) {
            continue;
        }
        if (externalPlugins.contains(externalPlugin)) {
            continue;
        }

        externalPlugins << externalPlugin;
        externalPluginPaths << externalPluginDir.absoluteFilePath(externalPlugin);
    }
    externalPlugins.clear();

    for (const QString& pluginPath : std::as_const(pluginPaths)) {
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
                Writer writer;
                writer.d->m_plugin = plugin;
                writer.setAutoDeletePlugin(Writer::DoNotDeletePlugin);

                m_allWriters.push_back(std::move(writer));
            } else {
                qCDebug(KFILEMETADATA_LOG) << "Plugin could not be converted to a WriterPlugin";
                qCDebug(KFILEMETADATA_LOG) << pluginPath;
            }
        }
        else {
            qCDebug(KFILEMETADATA_LOG) << "Plugin could not create instance" << pluginPath;
        }
    }

    for (const QString& externalPluginPath : std::as_const(externalPluginPaths)) {
        ExternalWriter *plugin = new ExternalWriter(externalPluginPath);
        Writer writer;
        writer.d->m_plugin = plugin;
        writer.setAutoDeletePlugin(Writer::AutoDeletePlugin);

        m_allWriters.push_back(std::move(writer));
    }

    for (Writer& writer : m_allWriters) {
        const QStringList lst = writer.mimetypes();
        for (const QString& type : lst) {
            m_writers.insert(type, &writer);
        }
    }
}

QList<Writer*> WriterCollection::fetchWriters(const QString& mimetype) const
{
    QList<Writer*> plugins = d->m_writers.values(mimetype);
    if (!plugins.isEmpty()) {
        return plugins;
    }

    // try to find the best matching more generic writer by mimetype inheritance
    QMimeDatabase db;
    auto type = db.mimeTypeForName(mimetype);
    const QStringList ancestors = type.allAncestors();

    for (const auto &ancestor : ancestors) {
        if (ancestor == QLatin1String("application/octet-stream")) {
            continue;
        }
        QList<Writer*> plugins = d->m_writers.values(ancestor);
        if (!plugins.isEmpty()) {
            qCDebug(KFILEMETADATA_LOG) << "Using inherited mimetype" << ancestor <<  "for" << mimetype;
            return plugins;
        }
    }

    return plugins;
}



