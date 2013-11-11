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
#include "extractorpluginmanager.h"

#include <KService>
#include <KMimeType>
#include <KServiceTypeTrader>
#include <KDebug>

namespace Nepomuk2
{


ExtractorPluginManager::ExtractorPluginManager(QObject* parent): QObject(parent)
{
    QList< ExtractorPlugin* > all = allExtractors();

    foreach(ExtractorPlugin * ex, all) {
        if (ex->criteria() == ExtractorPlugin::BasicMimeType) {
            foreach(const QString & type, ex->mimetypes()) {
                m_simpleExtractors.insertMulti(type, ex);
            }
        } else if (ex->criteria() == ExtractorPlugin::Custom) {
            m_complexExtractors << ex;
        }
    }
}

ExtractorPluginManager::~ExtractorPluginManager()
{
    qDeleteAll(m_simpleExtractors.values().toSet());
    qDeleteAll(m_complexExtractors);
}


QList<ExtractorPlugin*> ExtractorPluginManager::allExtractors()
{
    // Get all the plugins
    KService::List plugins = KServiceTypeTrader::self()->query("NepomukFileExtractor");

    QList<ExtractorPlugin*> extractors;
    KService::List::const_iterator it;
    for (it = plugins.constBegin(); it != plugins.constEnd(); it++) {
        KService::Ptr service = *it;

        QString error;
        Nepomuk2::ExtractorPlugin* ex = service->createInstance<Nepomuk2::ExtractorPlugin>(this, QVariantList(), &error);
        if (!ex) {
            kError() << "Could not create Extractor: " << service->library();
            kError() << error;
            continue;
        }

        extractors << ex;
    }

    return extractors;
}

QList<ExtractorPlugin*> ExtractorPluginManager::fetchExtractors(const QUrl& url, const QString& mimetype)
{
    QList<ExtractorPlugin*> plugins = m_simpleExtractors.values(mimetype);
    foreach(ExtractorPlugin * ex, m_complexExtractors) {
        if (ex->shouldExtract(url, mimetype))
            plugins << ex;
    }

    return plugins;
}

}
