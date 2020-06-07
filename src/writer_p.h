/*
    This file is part of the KFileMetaData project
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2016 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef KFILEMETADATA_WRITER_P_H
#define KFILEMETADATA_WRITER_P_H

#include "writerplugin.h"

namespace KFileMetaData {

class WriterPlugin;

class Writer::WriterPrivate
{
public:
    ~WriterPrivate() {
        if (m_autoDeletePlugin == AutoDeletePlugin) {
            delete m_plugin;
        }
    }

    WriterPlugin *m_plugin = nullptr;

    WriterPluginOwnership m_autoDeletePlugin = AutoDeletePlugin;
};

}

#endif

