/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
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
    delete d;
}

Extractor::Extractor(Extractor&& other)
{
    d = other.d;
    other.d = nullptr;
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
