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

#ifndef EXTRACTORPLUGINMANAGER_H
#define EXTRACTORPLUGINMANAGER_H

#include <QtCore/QUrl>

namespace KMetaData
{

class ExtractorPlugin;

class ExtractorPluginManager : public QObject
{
public:
    explicit ExtractorPluginManager(QObject* parent = 0);
    virtual ~ExtractorPluginManager();

    /**
     * Fetch the extractors which can be used to extract
     * data for the respective file with the given mimetype
     */
    QList<ExtractorPlugin*> fetchExtractors(const QUrl& url, const QString& mimetype);

private:
    QHash<QString, ExtractorPlugin*> m_simpleExtractors;
    QList<ExtractorPlugin*> m_complexExtractors;


    QList<ExtractorPlugin*> allExtractors();
};
}

#endif // EXTRACTORPLUGINMANAGER_H
