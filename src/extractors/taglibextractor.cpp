/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>

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


#include "taglibextractor.h"

// Taglib includes
#include <fileref.h>
#include <tfilestream.h>
#include <flacfile.h>
#include <apetag.h>
#include <mpcfile.h>
#include <id3v2tag.h>
#include <id3v1genres.h>
#include <mpegfile.h>
#include <oggfile.h>
#include <mp4file.h>
#include <mp4tag.h>
#include <taglib.h>
#include <tag.h>
#include <vorbisfile.h>
#include <opusfile.h>
#include <xiphcomment.h>

#include <QDateTime>
#include <QDebug>

using namespace KFileMetaData;

static QString convertWCharsToQString(const TagLib::String& t)
{
    return QString::fromWCharArray((const wchar_t*)t.toCWString(), t.length());
}

TagLibExtractor::TagLibExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{
}

QStringList TagLibExtractor::mimetypes() const
{
    return QStringList{
        QStringLiteral("audio/mpeg"),
        QStringLiteral("audio/mpeg3"), 
        QStringLiteral("audio/x-mpeg"),
        QStringLiteral("audio/mp4"),
        QStringLiteral("audio/flac"),
        QStringLiteral("audio/x-musepack"),
        QStringLiteral("audio/ogg"), 
        QStringLiteral("audio/x-vorbis+ogg"),
        QStringLiteral("audio/opus"), 
        QStringLiteral("audio/x-opus+ogg"),
        QStringLiteral("audio/wav"),
        QStringLiteral("audio/x-aiff"),
        QStringLiteral("audio/x-ape"),
        QStringLiteral("audio/x-wavpack")
    };
}

void TagLibExtractor::extractMP3(TagLib::FileStream& stream, ExtractedData& data)
{
    // Handling multiple tags in mpeg files.
    TagLib::MPEG::File mpegFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
    if (!mpegFile.ID3v2Tag() || mpegFile.ID3v2Tag()->isEmpty()) {
        return;
    }
    
    TagLib::ID3v2::FrameList lstID3v2;

    // Artist.
    lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TPE1"];
    if (!lstID3v2.isEmpty()) {
        for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
            if (!data.artists.isEmpty()) {
                data.artists += ", ";
            }
            data.artists += (*it)->toString();
        }
    }

    // Album Artist.
    lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TPE2"];
    if (!lstID3v2.isEmpty()) {
        for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
            if (!data.albumArtists.isEmpty()) {
                data.albumArtists += ", ";
            }
            data.albumArtists += (*it)->toString();
        }
    }

    // Composer.
    lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TCOM"];
    if (!lstID3v2.isEmpty()) {
        for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
            if (!data.composers.isEmpty()) {
                data.composers += ", ";
            }
            data.composers += (*it)->toString();
        }
    }

    // Lyricist.
    lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TEXT"];
    if (!lstID3v2.isEmpty()) {
        for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
            if (!data.lyricists.isEmpty()) {
                data.lyricists += ", ";
            }
            data.lyricists += (*it)->toString();
        }
    }

    // Genre.
    lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TCON"];
    if (!lstID3v2.isEmpty()) {
        for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
            data.genres.append((*it)->toString());
        }
    }

    // Disc number.
    lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TPOS"];
    if (!lstID3v2.isEmpty()) {
        for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
            data.discNumber = (*it)->toString().toInt();
        }
    }
}

void TagLibExtractor::extractMP4(TagLib::FileStream& stream, ExtractedData& data)
{
    TagLib::MP4::File mp4File(&stream, true);
    if (!mp4File.tag() || mp4File.tag()->isEmpty()) {
        return;
    }
    TagLib::MP4::ItemListMap allTags = mp4File.tag()->itemListMap();

    TagLib::MP4::ItemListMap::Iterator itAlbumArtists = allTags.find("aART");
    if (itAlbumArtists != allTags.end()) {
        data.albumArtists = itAlbumArtists->second.toStringList().toString(", ");
    }

    TagLib::MP4::ItemListMap::Iterator itDiscNumber = allTags.find("disk");
    if (itDiscNumber != allTags.end()) {
        data.discNumber = itDiscNumber->second.toInt();
    }

    TagLib::String composerAtomName(TagLib::String("©wrt", TagLib::String::UTF8).to8Bit(), TagLib::String::Latin1);

    TagLib::MP4::ItemListMap::Iterator itComposers = allTags.find(composerAtomName);
    if (itComposers != allTags.end()) {
        data.composers = itComposers->second.toStringList().toString(", ");
    }
}

void TagLibExtractor::extractMusePack(TagLib::FileStream& stream, ExtractedData& data)
{
    TagLib::MPC::File mpcFile(&stream, true);
    if (!mpcFile.tag() || mpcFile.tag()->isEmpty()) {
        return;
    }
    
    TagLib::APE::ItemListMap lstMusepack = mpcFile.APETag()->itemListMap();
    TagLib::APE::ItemListMap::ConstIterator itMPC;

    itMPC = lstMusepack.find("ARTIST");
    if (itMPC != lstMusepack.end()) {
        if (!data.artists.isEmpty()) {
            data.artists += ", ";
        }
        data.artists += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("ALBUM ARTIST");
    if (itMPC != lstMusepack.end()) {
        if(!data.albumArtists.isEmpty()) {
            data.albumArtists += ", ";
        }
        data.albumArtists += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("COMPOSER");
    if (itMPC != lstMusepack.end()) {
        if (!data.composers.isEmpty()) {
            data.composers += ", ";
        }
        data.composers += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("LYRICIST");
    if (itMPC != lstMusepack.end()) {
        if (!data.lyricists.isEmpty()) {
            data.lyricists += ", ";
        }
        data.lyricists += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("GENRE");
    if (itMPC != lstMusepack.end()) {
        data.genres.append((*itMPC).second.toString());
    }
}

void TagLibExtractor::extractOgg(TagLib::FileStream& stream, const QString& mimeType, ExtractedData& data)
{
    TagLib::Ogg::FieldListMap lstOgg;

    if (mimeType == QStringLiteral("audio/flac")) {
        TagLib::FLAC::File flacFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        if (flacFile.xiphComment() && !flacFile.xiphComment()->isEmpty()) {
            lstOgg = flacFile.xiphComment()->fieldListMap();
        }
    }

    // Vorbis files.
    if (mimeType == QStringLiteral("audio/ogg") || mimeType == QStringLiteral("audio/x-vorbis+ogg")) {
        TagLib::Ogg::Vorbis::File oggFile(&stream, true);
        if (oggFile.tag() && !oggFile.tag()->isEmpty()) {
            lstOgg = oggFile.tag()->fieldListMap();
        }
    }

    if (mimeType == QStringLiteral("audio/opus") || mimeType == QStringLiteral("audio/x-opus+ogg")) {
        TagLib::Ogg::Opus::File opusFile(&stream, true);
        if (opusFile.tag() && !opusFile.tag()->isEmpty()) {
            lstOgg = opusFile.tag()->fieldListMap();
        }
    }

    // Handling OGG container tags.
    if (!lstOgg.isEmpty()) {
        TagLib::Ogg::FieldListMap::ConstIterator itOgg;

        itOgg = lstOgg.find("ARTIST");
        if (itOgg != lstOgg.end()) {
            if (!data.artists.isEmpty()) {
                data.artists += ", ";
            }
            data.artists += (*itOgg).second.toString(", ");
        }

        itOgg = lstOgg.find("ALBUMARTIST");
        if (itOgg != lstOgg.end()) {
            if (!data.albumArtists.isEmpty()) {
                data.albumArtists += ", ";
            }
            data.albumArtists += (*itOgg).second.toString(", ");
        }

        itOgg = lstOgg.find("COMPOSER");
        if (itOgg != lstOgg.end()) {
            if (!data.composers.isEmpty()) {
                data.composers += ", ";
            }
            data.composers += (*itOgg).second.toString(", ");
        }

        itOgg = lstOgg.find("LYRICIST");
        if (itOgg != lstOgg.end()) {
            if (!data.lyricists.isEmpty()) {
                data.lyricists += ", ";
            }
            data.lyricists += (*itOgg).second.toString(", ");
        }

        itOgg = lstOgg.find("GENRE");
        if (itOgg != lstOgg.end()) {
            data.genres.append((*itOgg).second);
        }

        itOgg = lstOgg.find("DISCNUMBER");
        if (itOgg != lstOgg.end()) {
            data.discNumber = (*itOgg).second.toString("").toInt();
        }
    }
}

void TagLibExtractor::extract(ExtractionResult* result)
{
    const QString fileUrl = result->inputUrl();
    const QString mimeType = result->inputMimetype();

    // Open the file readonly. Important if we're sandboxed.
    TagLib::FileStream stream(fileUrl.toUtf8().constData(), true);
    if (!stream.isOpen()) {
        qWarning() << "Unable to open file readonly: " << fileUrl;
        return;
    }

#if (TAGLIB_MAJOR_VERSION > 1) || (TAGLIB_MAJOR_VERSION == 1 && TAGLIB_MINOR_VERSION >= 11)
    TagLib::FileRef file(&stream, true);
#else
    TagLib::FileRef file(stream.name());
#endif
    if (file.isNull()) {
        qWarning() << "Unable to open file: " << fileUrl;
        return;
    }

    TagLib::Tag* tags = file.tag();
    result->addType(Type::Audio);

    ExtractedData data;
    
    if ((mimeType == QStringLiteral("audio/mpeg")) 
        || (mimeType == QStringLiteral("audio/mpeg3")) 
        || (mimeType == QStringLiteral("audio/x-mpeg"))) {
        extractMP3(stream, data);
    } else if (mimeType == QStringLiteral("audio/mp4")) {
        extractMP4(stream, data);
    } else if (mimeType == QStringLiteral("audio/x-musepack")) {
        extractMusePack(stream, data);
    } else {
        extractOgg(stream, mimeType, data);
    }

    if (!tags->isEmpty()) {
        QString title = convertWCharsToQString(tags->title());
        if (!title.isEmpty()) {
            result->add(Property::Title, title);
        }

        QString comment = convertWCharsToQString(tags->comment());
        if (!comment.isEmpty()) {
            result->add(Property::Comment, comment);
        }

        if (data.genres.isEmpty()) {
            data.genres.append(tags->genre());
        }

        for (uint i = 0; i < data.genres.size(); i++) {
            QString genre = convertWCharsToQString(data.genres[i]).trimmed();

            // Convert from int
            bool ok = false;
            int genreNum = genre.toInt(&ok);
            if (ok) {
                genre = convertWCharsToQString(TagLib::ID3v1::genre(genreNum));
            }

            result->add(Property::Genre, genre);
        }

        const auto artistString = data.artists.isEmpty() 
            ? convertWCharsToQString(tags->artist()) 
            : convertWCharsToQString(data.artists).trimmed();
        const auto artists = contactsFromString(artistString);
        for (auto& artist : artists) {
            result->add(Property::Artist, artist);
        }

        const auto  composersString = convertWCharsToQString(data.composers).trimmed();
        const auto composers = contactsFromString(composersString);
        for (auto& comp : composers) {
            result->add(Property::Composer, comp);
        }

        const auto lyricistsString = convertWCharsToQString(data.lyricists).trimmed();
        const auto lyricists = contactsFromString(lyricistsString);
        for (auto& lyr : lyricists) {
            result->add(Property::Lyricist, lyr);
        }

        const auto album = convertWCharsToQString(tags->album());
        if (!album.isEmpty()) {
            result->add(Property::Album, album);

            const auto albumArtistsString = convertWCharsToQString(data.albumArtists).trimmed();
            const auto albumArtists = contactsFromString(albumArtistsString);
            for (auto& res : albumArtists) {
                result->add(Property::AlbumArtist, res);
            }
        }

        if (tags->track()) {
            result->add(Property::TrackNumber, tags->track());
        }

        if (tags->year()) {
            result->add(Property::ReleaseYear, tags->year());
        }
    }

    if (data.discNumber.isValid()) {
        result->add(Property::DiscNumber, data.discNumber);
    }

    TagLib::AudioProperties* audioProp = file.audioProperties();
    if (audioProp) {
        if (audioProp->length()) {
            // What about the xml duration?
            result->add(Property::Duration, audioProp->length());
        }

        if (audioProp->bitrate()) {
            result->add(Property::BitRate, audioProp->bitrate() * 1000);
        }

        if (audioProp->channels()) {
            result->add(Property::Channels, audioProp->channels());
        }

        if (audioProp->sampleRate()) {
            result->add(Property::SampleRate, audioProp->sampleRate());
        }
    }

    // TODO: Get more properties based on the file type
    // - Codec
    // - Album Artist
    // - Publisher

    // TAG information (incomplete).
    // A good reference: http://qoobar.sourceforge.net/en/documentation.htm
    // -- FLAC/OGG --
    // Artist:          ARTIST, PERFORMER
    // Album artist:    ALBUMARTIST
    // Composer:        COMPOSER
    // Lyricist:        LYRICIST
    // Conductor:       CONDUCTOR
    // Disc number:     DISCNUMBER
    // Total discs:     TOTALDISCS, DISCTOTAL
    // Track number:    TRACKNUMBER
    // Total tracks:    TOTALTRACKS, TRACKTOTAL
    // Genre: GENRE
    // -- ID3v2 --
    // Artist:                          TPE1
    // Album artist:                    TPE2
    // Composer:                        TCOM
    // Lyricist:                        TEXT
    // Conductor:                       TPE3
    // Disc number[/total dics]:        TPOS
    // Track number[/total tracks]:     TRCK
    // Genre:                           TCON
}
