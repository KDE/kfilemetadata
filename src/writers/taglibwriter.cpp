/*
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "taglibwriter.h"
#include "kfilemetadata_debug.h"

#include <taglib.h>
#include <tfilestream.h>
#include <tpropertymap.h>
#include <tstring.h>
#include <aifffile.h>
#include <apefile.h>
#include <asffile.h>
#include <asftag.h>
#include <flacfile.h>
#include <mp4file.h>
#include <mp4tag.h>
#include <mpcfile.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <oggfile.h>
#include <opusfile.h>
#include <vorbisfile.h>
#include <speexfile.h>
#include <wavpackfile.h>
#include <wavfile.h>
#include <popularimeterframe.h>

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
    QStringLiteral("audio/x-aifc"),
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

int id3v2RatingTranslation[11] = {
    0, 1, 13, 54, 64, 118, 128, 186, 196, 242, 255
};


using namespace KFileMetaData;

void writeID3v2Tags(TagLib::ID3v2::Tag *id3Tags, const PropertyMap &newProperties)
{
    if (newProperties.contains(Property::Rating)) {
        int rating = newProperties.value(Property::Rating).toInt();
        if (rating >= 0 && rating <= 10) {
            id3Tags->removeFrames("POPM");
            auto ratingFrame = new TagLib::ID3v2::PopularimeterFrame;
            ratingFrame->setEmail("org.kde.kfilemetadata");
            ratingFrame->setRating(id3v2RatingTranslation[rating]);
            id3Tags->addFrame(ratingFrame);
        }
    }
}

void writeApeTags(TagLib::PropertyMap &oldProperties, const PropertyMap &newProperties)
{
    if (newProperties.contains(Property::Rating)) {
        oldProperties.replace("RATING", TagLib::String::number(newProperties.value(Property::Rating).toInt() * 10));
    }
}

void writeVorbisTags(TagLib::PropertyMap &oldProperties, const PropertyMap &newProperties)
{
    if (newProperties.contains(Property::Rating)) {
        oldProperties.replace("RATING", TagLib::String::number(newProperties.value(Property::Rating).toInt() * 10));
    }
}

void writeAsfTags(TagLib::ASF::Tag *asfTags, const PropertyMap &properties)
{
    if (properties.contains(Property::Rating)) {
        //map the rating values of WMP to Baloo rating
        //0->0, 1->2, 4->25, 6->50, 8->75, 10->99
        int rating = properties.value(Property::Rating).toInt();
        if (rating == 0) {
            rating = 0;
        } else if (rating <= 2) {
            rating = 1;
        } else if (rating == 10){
            rating = 99;
        } else {
            rating = static_cast<int>(12.5 * rating - 25);
        }
        asfTags->setAttribute("WM/SharedUserRating", TagLib::String::number(rating));
    }
}

void writeMp4Tags(TagLib::MP4::Tag *mp4Tags, const PropertyMap &newProperties)
{
    if (newProperties.contains(Property::Rating)) {
        mp4Tags->setItem("rate", TagLib::StringList(TagLib::String::number(newProperties.value(Property::Rating).toInt() * 10)));
    }
}

} // anonymous namespace

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

#if defined Q_OS_WINDOWS
    TagLib::FileStream stream(fileUrl.toLocal8Bit().constData(), true);
#else
    TagLib::FileStream stream(fileUrl.toUtf8().constData(), false);
#endif
    if (!stream.isOpen()) {
        qCWarning(KFILEMETADATA_LOG) << "Unable to open file in write mode: " << fileUrl;
        return;
    }

    if (mimeType == QLatin1String("audio/mpeg") || mimeType == QLatin1String("audio/mpeg3")
            || mimeType == QLatin1String("audio/x-mpeg")) {
        TagLib::MPEG::File file(&stream, TagLib::ID3v2::FrameFactory::instance(), false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            if (file.hasID3v2Tag()) {
                writeID3v2Tags(file.ID3v2Tag(), properties);
            }
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-aiff") || mimeType == QLatin1String("audio/x-aifc"))  {
        TagLib::RIFF::AIFF::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            auto id3Tags = dynamic_cast<TagLib::ID3v2::Tag*>(file.tag());
            if (id3Tags) {
                writeID3v2Tags(id3Tags, properties);
            }
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/wav") || mimeType == QLatin1String("audio/x-wav")) {
        TagLib::RIFF::WAV::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            auto id3Tags = dynamic_cast<TagLib::ID3v2::Tag*>(file.tag());
            if (id3Tags) {
                writeID3v2Tags(id3Tags, properties);
            }
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-musepack")) {
        TagLib::MPC::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeApeTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-ape")) {
        TagLib::APE::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeApeTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-wavpack")) {
        TagLib::WavPack::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeApeTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/mp4")) {
        TagLib::MP4::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            auto mp4Tags = dynamic_cast<TagLib::MP4::Tag*>(file.tag());
            if (mp4Tags) {
                writeMp4Tags(mp4Tags, properties);
            }
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/flac")) {
        TagLib::FLAC::File file(&stream, TagLib::ID3v2::FrameFactory::instance(), false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeVorbisTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/ogg") || mimeType == QLatin1String("audio/x-vorbis+ogg")) {
        TagLib::Ogg::Vorbis::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeVorbisTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/opus") || mimeType == QLatin1String("audio/x-opus+ogg")) {
        TagLib::Ogg::Opus::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeVorbisTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/speex") || mimeType == QLatin1String("audio/x-speex+ogg")) {
        TagLib::Ogg::Speex::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            writeVorbisTags(savedProperties, properties);
            file.setProperties(savedProperties);
            file.save();
        }
    } else if (mimeType == QLatin1String("audio/x-ms-wma")) {
        TagLib::ASF::File file(&stream, false);
        if (file.isValid()) {
            auto savedProperties = file.properties();
            writeGenericProperties(savedProperties, properties);
            file.setProperties(savedProperties);
            auto asfTags = dynamic_cast<TagLib::ASF::Tag*>(file.tag());
            if (asfTags){
                writeAsfTags(asfTags, properties);
            }
            file.save();
        }
    }
}
