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

#include "extractor.h"
#include "extractor_p.h"
#include "extractorplugin.h"

using namespace KFileMetaData;

Extractor::Extractor()
    : d(new ExtractorPrivate)
{
}

Extractor::~Extractor()
{
    if (d->m_autoDeletePlugin == AutoDeletePlugin) {
        delete d->m_plugin;
    }

    delete d;
}

void Extractor::extract(ExtractionResult* result)
{
    d->m_plugin->extract(result);
}

QStringList Extractor::mimetypes() const
{
    return d->m_plugin->mimetypes();
}

QVariantMap Extractor::extractorProperties() const
{
    return d->m_metaData;
}

void Extractor::setExtractorPlugin(ExtractorPlugin *extractorPlugin)
{
    d->m_plugin = extractorPlugin;
}

void Extractor::setAutoDeletePlugin(ExtractorPluginOwnership autoDelete)
{
    d->m_autoDeletePlugin = autoDelete;
}

void Extractor::setMetaData(const QVariantMap &metaData)
{
    d->m_metaData = metaData;
}
