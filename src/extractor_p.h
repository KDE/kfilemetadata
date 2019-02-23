/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
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

#ifndef KFILEMETADATA_EXTRACTOR_P_H
#define KFILEMETADATA_EXTRACTOR_P_H

#include "extractorplugin.h"
#include "kfilemetadata_debug.h"
#include <QPluginLoader>

namespace KFileMetaData {

class ExtractorPlugin;

class Extractor::ExtractorPrivate
{
public:
    ~ExtractorPrivate() {
        if (m_autoDeletePlugin == AutoDeletePlugin) {
            delete m_plugin;
        }
    }
    bool initPlugin();

    ExtractorPlugin *m_plugin = nullptr;

    ExtractorPluginOwnership m_autoDeletePlugin = AutoDeletePlugin;

    QVariantMap m_metaData;
    QString m_pluginPath;
};

inline bool Extractor::ExtractorPrivate::initPlugin()
{
    if (m_plugin) {
        return true;
    }

    QPluginLoader loader(m_pluginPath);
    if (!loader.load()) {
        qCWarning(KFILEMETADATA_LOG) << "Could not create Extractor:" << m_pluginPath;
        qCWarning(KFILEMETADATA_LOG) << loader.errorString();
        return false;
    }

    QObject* obj = loader.instance();
    if (!obj) {
        qCWarning(KFILEMETADATA_LOG) << "Could not create instance:" << m_pluginPath;
        return false;
    }

    m_plugin = qobject_cast<ExtractorPlugin*>(obj);
    if (!m_plugin) {
        qCWarning(KFILEMETADATA_LOG) << "Could not convert to ExtractorPlugin:" << m_pluginPath;
        return false;
    }

    m_autoDeletePlugin = Extractor::DoNotDeletePlugin;
    return true;
}

}

#endif
