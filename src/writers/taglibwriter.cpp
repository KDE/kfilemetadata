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
#include <tfilestream.h>
#include <tpropertymap.h>
#include <tstring.h>
#include <aifffile.h>
#include <apefile.h>
#include <asffile.h>
#include <flacfile.h>
#include <mp4file.h>
#include <mpcfile.h>
#include <mpegfile.h>
#include <oggfile.h>
#include <opusfile.h>
#include <vorbisfile.h>
#include <speexfile.h>
#include <wavpackfile.h>
#include <wavfile.h>

#include <QDebug>

namespace {

const QStringList supportedMimeTypes = {
    QStringLiteral("audio/flac"),
    QStringLiteral("audio/mp4"),
    QStringLiteral("audio/mpeg"),
    QStringLiteral("audio/mpeg3"),
    QStringLiteral("audio/ogg"),
    QStringLiteral("audio/opus"),
    QStringLiteral("audio/speex"),
    QStringLiteral("audio/wav"),
    QStringLiteral("audio/x-aiff"),
    QStringLiteral("audio/x-ape"),
    QStringLiteral("audio/x-mpeg"),
    QStringLiteral("audio/x-ms-wma"),
    QStringLiteral("audio/x-musepack"),
    QStringLiteral("audio/x-opus+ogg"),
    QStringLiteral("audio/x-speex+ogg"),
    QStringLiteral("audio/x-vorbis+ogg"),
    QStringLiteral("audio/x-wav"),
    QStringLiteral("audio/x-wavpack"),
};

}

using namespace KFileMetaData;

void writeGenericProperties(TagLib::PropertyMap &oldProperties, const PropertyMap &newProperties)
{
    if (newProperties.contains(Property::Title)) {
        oldProperties.replace("TITLE", QStringToTString(newProperties.value(Property::Title).toString()));
    }

    if (newProperties.contains(Property::Artist)) {
        oldProperties.replace("ARTIST", QStringToTString(newProperties.value(Property::Artist).toString()));
    }

    if (newProperties.contains(Property::AlbumArtist)) {
        oldProperties.replace("ALBUMARTIST", QStringToTString(newProperties.value(Property::AlbumArtist).toString()));
    }

    if (newProperties.contains(Property::Album)) {
        oldProperties.replace("ALBUM", QStringToTString(newProperties.value(Property::Album).toString()));
    }

    if (newProperties.contains(Property::TrackNumber)) {
        int trackNumber = newProperties.value(Property::TrackNumber).toInt();
        //taglib requires uint
        if (trackNumber >= 0) {
            oldProperties.replace("TRACKNUMBER", QStringToTString(newProperties.value(Property::TrackNumber).toString()));
        }
    }

    if (newProperties.contains(Property::DiscNumber)) {
        int discNumber = newProperties.value(Property::DiscNumber).toInt();
        //taglib requires uint
        if (discNumber >= 0) {
            oldProperties.replace("DISCNUMBER", QStringToTString(newProperties.value(Property::DiscNumber).toString()));
        }
    }

    if (newProperties.contains(Property::ReleaseYear)) {
        int year = newProperties.value(Property::ReleaseYear).toInt();
        //taglib requires uint
        if (year >= 0) {
            oldProperties.replace("DATE", QStringToTString(newProperties.value(Property::ReleaseYear).toString()));
        }
    }

    if (newProperties.contains(Property::Genre)) {
        oldProperties.replace("GENRE", QStringToTString(newProperties.value(Property::Genre).toString()));
    }

    if (newProperties.contains(Property::Comment)) {
        oldProperties.replace("COMMENT", QStringToTString(newProperties.value(Property::Comment).toString()));
    }

    if (newProperties.contains(Property::Composer)) {
        oldProperties.replace("COMPOSER", QStringToTString(newProperties.value(Property::Composer).toString()));
    }

    if (newProperties.contains(Property::Lyricist)) {
        oldProperties.replace("LYRICIST", QStringToTString(newProperties.value(Property::Lyricist).toString()));
    }

    if (newProperties.contains(Property::Conductor)) {
        oldProperties.replace("CONDUCTOR", QStringToTString(newProperties.value(Property::Conductor).toString()));
    }

    if (newProperties.contains(Property::Copyright)) {
        oldProperties.replace("COPYRIGHT", QStringToTString(newProperties.value(Property::Copyright).toString()));
    }

    if (newProperties.contains(Property::Lyrics)) {
        oldProperties.replace("LYRICS", QStringToTString(newProperties.value(Property::Lyrics).toString()));
    }

    if (newProperties.contains(Property::Language)) {
        oldProperties.replace("LANGUAGE", QStringToTString(newProperties.value(Property::Language).toString()));
    }
}

TagLibWriter::TagLibWriter(QObject* parent)
    : WriterPlugin(parent)
{
}

QStringList TagLibWriter::writeMimetypes() const
{
    return supportedMimeTypes;
}

void TagLibWriter::write(const WriteData& data)
{
    const QString fileUrl = data.inputUrl();
    const PropertyMap properties = data.getAllProperties();
    const QString mimeType = data.inputMimetype();

    TagLib::FileStream stream(fileUrl.toUtf8().constData(), false);
    if (!stream.isOpen()) {
        qWarning() << "Unable to open file in write mode: " << fileUrl;
        return;
    }

    if ((mimeType == QLatin1String("audio/mpeg")) || (mimeType == QLatin1String("audio/mpeg3"))
            || (mimeType == QLatin1String("audio/x-mpeg"))) {
        TagLib::MPEG::File file(&stream, TagLib::ID3v2::FrameFactory::instance(), false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-aiff")) {
        TagLib::RIFF::AIFF::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if ((mimeType == QLatin1String("audio/wav")) || (mimeType == QLatin1String("audio/x-wav"))) {
        TagLib::RIFF::WAV::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-musepack")) {
        TagLib::MPC::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-ape")) {
        TagLib::APE::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-wavpack")) {
        TagLib::WavPack::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/mp4")) {
        TagLib::MP4::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/flac")) {
        TagLib::FLAC::File file(&stream, TagLib::ID3v2::FrameFactory::instance(), false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/ogg") || mimeType == QLatin1String("audio/x-vorbis+ogg")) {
        TagLib::Ogg::Vorbis::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/opus") || mimeType == QLatin1String("audio/x-opus+ogg")) {
        TagLib::Ogg::Opus::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/speex") || mimeType == QLatin1String("audio/x-speex+ogg")) {
        TagLib::Ogg::Speex::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-ms-wma")) {
        TagLib::ASF::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    }
}
