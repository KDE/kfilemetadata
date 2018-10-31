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

#include "taglibwriter.h"

#include <taglib.h>
#include <tstring.h>
#include <id3v2tag.h>
#include <fileref.h>

using namespace KFileMetaData;

TagLibWriter::TagLibWriter(QObject* parent)
    : WriterPlugin(parent)
{
}

QStringList TagLibWriter::writeMimetypes() const
{
    QStringList types = {
        QStringLiteral("audio/flac"),
        QStringLiteral("audio/mp4"),
        QStringLiteral("audio/mpeg"),
        QStringLiteral("audio/mpeg3"),
        QStringLiteral("audio/ogg"),
        QStringLiteral("audio/opus"),
        QStringLiteral("audio/x-mpeg"),
        QStringLiteral("audio/x-musepack"),
        QStringLiteral("audio/x-opus+ogg"),
        QStringLiteral("audio/x-vorbis+ogg"),
    };

    return types;
}

void TagLibWriter::write(const WriteData& data)
{
    const QString fileUrl = data.inputUrl();
    const PropertyMap properties = data.getAllProperties();

    TagLib::FileRef file(fileUrl.toUtf8().constData(), true);
    if (file.isNull()) {
        return;
    }

    TagLib::Tag* tags = file.tag();

    TagLib::String title;
    TagLib::String artist;
    TagLib::String album;
    TagLib::String genre;
    TagLib::String comment;

    if (properties.contains(Property::Title)) {
        title = QStringToTString(properties.value(Property::Title).toString());
        tags->setTitle(title);
    }

    if (properties.contains(Property::Artist)) {
        artist = QStringToTString(properties.value(Property::Artist).toString());
        tags->setArtist(artist);
    }

    if (properties.contains(Property::Album)) {
        album = QStringToTString(properties.value(Property::Album).toString());
        tags->setAlbum(album);
    }

    if (properties.contains(Property::TrackNumber)) {
        int trackNumber = properties.value(Property::TrackNumber).toInt();
        //taglib requires uint
        if (trackNumber >= 0) {
            tags->setTrack(trackNumber);
        }
    }

    if (properties.contains(Property::ReleaseYear)) {
        int year = properties.value(Property::ReleaseYear).toInt();
        //taglib requires uint
        if (year >= 0) {
            tags->setYear(year);
        }
    }

    if (properties.contains(Property::Genre)) {
        genre = QStringToTString(properties.value(Property::Genre).toString());
        tags->setGenre(genre);
    }

    if (properties.contains(Property::Comment)) {
        comment = QStringToTString(properties.value(Property::Comment).toString());
        tags->setComment(comment);
    }


    file.save();
}
