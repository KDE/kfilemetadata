/*
    This file is part of the KFileMetaData project that extract metadata from files
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>
    Copyright 2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef QTMULTIMEDIAEXTRACTOR_H
#define QTMULTIMEDIAEXTRACTOR_H

#include "extractorplugin.h"

#include <QThread>
#include <QSemaphore>
#include <QMutex>

namespace KFileMetaData
{

class ExecutionHelper;

class QtMultimediaExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    QtMultimediaExtractor(QObject* parent = 0);

    ~QtMultimediaExtractor();

    void extract(ExtractionResult* result) Q_DECL_OVERRIDE;
    QStringList mimetypes() const Q_DECL_OVERRIDE;

private:

    QThread mRunnerThread;

    QMutex mSynchronizeMutex;

    QSemaphore mSynchronizeSemaphore;

    ExecutionHelper *mHelper;

};

}

#endif // QTMULTIMEDIAEXTRACTOR_H
