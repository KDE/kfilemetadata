/*
 * Copyright (C) 2016 Varun Joshi <varunj.1011@gmail.com>
 * Copyright (C) 2018 Alexander Stippich <a.stippich@gmx.net>
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

#ifndef TAGLIBWRITER_H
#define TAGLIBWRITER_H

#include "writerplugin.h"

namespace KFileMetaData
{

class TagLibWriter : public WriterPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.WriterPlugin")
    Q_INTERFACES(KFileMetaData::WriterPlugin)

public:
    explicit TagLibWriter(QObject* parent = nullptr);

    void write(const WriteData& data) override;
    QStringList writeMimetypes() const override;
};

}

#endif // TAGLIBWRITER_H

