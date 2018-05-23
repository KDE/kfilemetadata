/*
 * This file is part of the KFileMetaData project
 * Copyright (C) 2016  Varun Joshi <varunj.1011@gmail.com>
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

#ifndef EXTERNALWRITER_H
#define EXTERNALWRITER_H

#include "writerplugin.h"

namespace KFileMetaData {

class ExternalWriter : public WriterPlugin
{

    Q_OBJECT

public:
    explicit ExternalWriter(QObject* parent = nullptr);
    explicit ExternalWriter(const QString& pluginPath);
    virtual ~ExternalWriter();

    void write(const WriteData& data) override;
    QStringList writeMimetypes() const override;

private:
    bool runtimeInstalled() const;
    bool dependenciesSatisfied() const;

    class ExternalWriterPrivate;
    ExternalWriterPrivate *d_ptr;
    Q_DECLARE_PRIVATE(ExternalWriter)
};
}

#endif // EXTERNALWRITER_H
