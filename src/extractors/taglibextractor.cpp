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
#include <flacfile.h>
#include <apetag.h>
#include <mpcfile.h>
#include <id3v2tag.h>
#include <id3v1genres.h>
#include <mpegfile.h>
#include <oggfile.h>
#include <taglib.h>
#include <tag.h>
#include <vorbisfile.h>
#include <opusfile.h>
#include <xiphcomment.h>

#include <QDateTime>

using namespace KFileMetaData;

TagLibExtractor::TagLibExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{
}

QStringList TagLibExtractor::mimetypes() const
{
    QStringList types;
    // MP3, FLAC, Vorbis, Opus,  MPC, Speex, WavPack TrueAudio, WAV, AIFF, MP4 and ASF files.
    // MP3
    types << QStringLiteral("audio/mpeg");
    types << QStringLiteral("audio/mpeg3"); types << QStringLiteral("audio/x-mpeg");

    // FLAC
    types << QStringLiteral("audio/flac");

    // MPC
    types << QStringLiteral("audio/x-musepack");

    // Vorbis
    types << QStringLiteral("audio/ogg"); types << QStringLiteral("audio/x-vorbis+ogg");

    // Opus
    types << QStringLiteral("audio/opus"); types << QStringLiteral("audio/x-opus+ogg");

    // WAV
    types << QStringLiteral("audio/wav");

    // AIFF
    types << QStringLiteral("audio/x-aiff");

    // APE
    types << QStringLiteral("audio/x-ape");

    // WV
    types << QStringLiteral("audio/x-wavpack");

    return types;
}

void TagLibExtractor::extract(ExtractionResult* result)
{
    const QString fileUrl = result->inputUrl();
    const QString mimeType = result->inputMimetype();

    TagLib::FileRef file(fileUrl.toUtf8().data(), true);
    if (file.isNull()) {
        return;
    }

    TagLib::Tag* tags = file.tag();
    result->addType(Type::Audio);

    TagLib::String artists;
    TagLib::String albumArtists;
    TagLib::String composers;
    TagLib::String lyricists;
    TagLib::StringList genres;

    // Handling multiple tags in mpeg files.
    if ((mimeType == "audio/mpeg") || (mimeType == "audio/mpeg3") || (mimeType == "audio/x-mpeg")) {
        TagLib::MPEG::File mpegFile(fileUrl.toUtf8().data(), true);
        if (mpegFile.ID3v2Tag() && !mpegFile.ID3v2Tag()->isEmpty()) {
            TagLib::ID3v2::FrameList lstID3v2;

            // Artist.
            lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TPE1"];
            if (!lstID3v2.isEmpty()) {
                for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
                    if (!artists.isEmpty()) {
                        artists += ", ";
                    }
                    artists += (*it)->toString();
                }
            }

            // Album Artist.
            lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TPE2"];
            if (!lstID3v2.isEmpty()) {
                for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
                    if (!albumArtists.isEmpty()) {
                        albumArtists += ", ";
                    }
                    albumArtists += (*it)->toString();
                }
            }

            // Composer.
            lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TCOM"];
            if (!lstID3v2.isEmpty()) {
                for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
                    if (!composers.isEmpty()) {
                        composers += ", ";
                    }
                    composers += (*it)->toString();
                }
            }

            // Lyricist.
            lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TEXT"];
            if (!lstID3v2.isEmpty()) {
                for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
                    if (!lyricists.isEmpty()) {
                        lyricists += ", ";
                    }
                    lyricists += (*it)->toString();
                }
            }

            // Genre.
            lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["TCON"];
            if (!lstID3v2.isEmpty()) {
                for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
                    genres.append((*it)->toString());
                }
            }

        }
    }

    // Handling multiple tags in Ogg containers.
    {
        TagLib::Ogg::FieldListMap lstOgg;

        // FLAC files.
        if (mimeType == "audio/flac") {
            TagLib::FLAC::File flacFile(fileUrl.toUtf8().data(), true);
            if (flacFile.xiphComment() && !flacFile.xiphComment()->isEmpty()) {
                lstOgg = flacFile.xiphComment()->fieldListMap();
            }
        }

        // Vorbis files.
        if (mimeType == "audio/ogg" || mimeType == "audio/x-vorbis+ogg") {
            TagLib::Ogg::Vorbis::File oggFile(fileUrl.toUtf8().data(), true);
            if (oggFile.tag() && !oggFile.tag()->isEmpty()) {
                lstOgg = oggFile.tag()->fieldListMap();
            }
        }

        // Opus files.
        if (mimeType == "audio/opus" || mimeType == "audio/x-opus+ogg") {
            TagLib::Ogg::Opus::File opusFile(fileUrl.toUtf8().data(), true);
            if (opusFile.tag() && !opusFile.tag()->isEmpty()) {
                lstOgg = opusFile.tag()->fieldListMap();
            }
        }

        // Handling OGG container tags.
        if (!lstOgg.isEmpty()) {
            TagLib::Ogg::FieldListMap::ConstIterator itOgg;

            // Artist.
            itOgg = lstOgg.find("ARTIST");
            if (itOgg != lstOgg.end()) {
                if (!artists.isEmpty()) {
                    artists += ", ";
                }
                artists += (*itOgg).second.toString(", ");
            }

            // Album Artist.
            itOgg = lstOgg.find("ALBUMARTIST");
            if (itOgg != lstOgg.end()) {
                if (!albumArtists.isEmpty()) {
                    albumArtists += ", ";
                }
                albumArtists += (*itOgg).second.toString(", ");
            }

            // Composer.
            itOgg = lstOgg.find("COMPOSER");
            if (itOgg != lstOgg.end()) {
                if (!composers.isEmpty()) {
                    composers += ", ";
                }
                composers += (*itOgg).second.toString(", ");
            }

            // Lyricist.
            itOgg = lstOgg.find("LYRICIST");
            if (itOgg != lstOgg.end()) {
                if (!lyricists.isEmpty()) {
                    lyricists += ", ";
                }
                lyricists += (*itOgg).second.toString(", ");
            }

            // Genre.
            itOgg = lstOgg.find("GENRE");
            if (itOgg != lstOgg.end()) {
                genres.append((*itOgg).second);
            }
        }
    }

    // Handling multiple tags in Musepack files.
    if (mimeType == ("audio/x-musepack")) {
        TagLib::MPC::File mpcFile(fileUrl.toUtf8().data(), true);
        if (mpcFile.tag() && !mpcFile.tag()->isEmpty()) {
            TagLib::APE::ItemListMap lstMusepack = mpcFile.APETag()->itemListMap();
            TagLib::APE::ItemListMap::ConstIterator itMPC;

            // Artist.
            itMPC = lstMusepack.find("ARTIST");
            if (itMPC != lstMusepack.end()) {
                if (!artists.isEmpty()) {
                    artists += ", ";
                }
                artists += (*itMPC).second.toString();
            }

            // Album Artist.
            itMPC = lstMusepack.find("ALBUMARTIST");
            if (itMPC != lstMusepack.end()) {
                if(!albumArtists.isEmpty()) {
                    albumArtists += ", ";
                }
                albumArtists += (*itMPC).second.toString();
            }

            // Composer.
            itMPC = lstMusepack.find("COMPOSER");
            if (itMPC != lstMusepack.end()) {
                if (!composers.isEmpty()) {
                    composers += ", ";
                }
                composers += (*itMPC).second.toString();
            }

            // Lyricist.
            itMPC = lstMusepack.find("LYRICIST");
            if (itMPC != lstMusepack.end()) {
                if (!lyricists.isEmpty()) {
                    lyricists += ", ";
                }
                lyricists += (*itMPC).second.toString();
            }

            // Genre.
            itMPC = lstMusepack.find("GENRE");
            if (itMPC != lstMusepack.end()) {
                genres.append((*itMPC).second.toString());
            }
        }
    }

    if (!tags->isEmpty()) {
        QString title = QString::fromUtf8(tags->title().toCString(true));
        if (!title.isEmpty()) {
            result->add(Property::Title, title);
        }

        QString comment = QString::fromUtf8(tags->comment().toCString(true));
        if (!comment.isEmpty()) {
            result->add(Property::Comment, comment);
        }

        if (genres.isEmpty()) {
            genres.append(tags->genre());
        }

        for (uint i = 0; i < genres.size(); i++) {
            QString genre = QString::fromUtf8(genres[i].toCString(true)).trimmed();

            // Convert from int
            bool ok = false;
            int genreNum = genre.toInt(&ok);
            if (ok) {
                genre = QString::fromUtf8(TagLib::ID3v1::genre(genreNum).toCString(true));
            }

            result->add(Property::Genre, genre);
        }

        QString artistString;
        if (artists.isEmpty()) {
            artistString = QString::fromUtf8(tags->artist().toCString(true));
        } else {
            artistString = QString::fromUtf8(artists.toCString(true)).trimmed();
        }

        QStringList artists = contactsFromString(artistString);
        foreach(const QString& artist, artists) {
            result->add(Property::Artist, artist);
        }

        QString composersString = QString::fromUtf8(composers.toCString(true)).trimmed();
        QStringList composers = contactsFromString(composersString);
        foreach(const QString& comp, composers) {
            result->add(Property::Composer, comp);
        }

        QString lyricistsString = QString::fromUtf8(lyricists.toCString(true)).trimmed();
        QStringList lyricists = contactsFromString(lyricistsString);
        foreach(const QString& lyr, lyricists) {
            result->add(Property::Lyricist, lyr);
        }

        QString album = QString::fromUtf8(tags->album().toCString(true));
        if (!album.isEmpty()) {
            result->add(Property::Album, album);

            QString albumArtistsString = QString::fromUtf8(albumArtists.toCString(true)).trimmed();
            QStringList albumArtists = contactsFromString(albumArtistsString);
            foreach(const QString& res, albumArtists) {
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
