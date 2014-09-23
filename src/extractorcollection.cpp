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

#include "extractorplugin.h"
#include "extractorcollection.h"

#include <QDebug>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDir>

using namespace KFileMetaData;

class ExtractorCollection::Private {
public:
    QHash<QString, ExtractorPlugin*> m_extractors;

    QList<ExtractorPlugin*> allExtractors() const;
};

ExtractorCollection::ExtractorCollection()
    : d(new Private)
{
    QList<ExtractorPlugin*> all = d->allExtractors();

    foreach (ExtractorPlugin* ex, all) {
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


QList<ExtractorPlugin*> ExtractorCollection::Private::allExtractors() const
{
    QStringList plugins;
    QStringList pluginPaths;

    QStringList paths = QCoreApplication::libraryPaths();
    Q_FOREACH (const QString& libraryPath, paths) {
        QString path(libraryPath + QStringLiteral("/kf5/kfilemetadata"));
        QDir dir(path);

        if (!dir.exists()) {
            continue;
        }

        QStringList entryList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
        Q_FOREACH (const QString& fileName, entryList) {
            // Make sure the same plugin is not loaded twice, even if it
            // installed in two different locations
            if (plugins.contains(fileName))
                continue;

            plugins << fileName;
            pluginPaths << dir.absoluteFilePath(fileName);
        }
    }
    plugins.clear();

    QList<ExtractorPlugin*> extractors;
    Q_FOREACH (const QString& pluginPath, pluginPaths) {
        QPluginLoader loader(pluginPath);

        if (!loader.load()) {
            qWarning() << "Could not create Extractor: " << pluginPath;
            qWarning() << loader.errorString();
            continue;
        }

        QObject* obj = loader.instance();
        if (obj) {
            ExtractorPlugin* ex = qobject_cast<ExtractorPlugin*>(obj);
            if (ex) {
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

    return extractors;
}

QList<ExtractorPlugin*> ExtractorCollection::fetchExtractors(const QString& mimetype) const
{
    QList<ExtractorPlugin*> plugins = d->m_extractors.values(mimetype);
    if (plugins.isEmpty()) {
        QHash<QString, ExtractorPlugin*>::const_iterator it = d->m_extractors.constBegin();
        for (; it != d->m_extractors.constEnd(); it++) {
            if (mimetype.startsWith(it.key()))
                plugins << it.value();
        }
    }

    return plugins;
}
