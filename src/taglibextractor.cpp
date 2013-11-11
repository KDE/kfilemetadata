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

#include "nie.h"
#include "nco.h"
#include "nmm.h"
#include "nfo.h"
#include <KDebug>

#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/id3v1genres.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/vorbisfile.h>
#include <taglib/xiphcomment.h>
#include <QDateTime>

using namespace Nepomuk2::Vocabulary;

namespace Nepomuk2
{

TagLibExtractor::TagLibExtractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{
}

QStringList TagLibExtractor::mimetypes()
{
    QStringList types;
    // MP3 FLAC, MPC, Speex, WavPack TrueAudio, WAV, AIFF, MP4 and ASF files.
    // MP3
    types << QLatin1String("audio/mpeg");
    types << QLatin1String("audio/mpeg3"); types << QLatin1String("audio/x-mpeg");

    // FLAC
    types << QLatin1String("audio/flac");

    //OGG
    types << QLatin1String("audio/ogg"); types << QLatin1String("audio/x-vorbis+ogg");

    // WAV
    types << QLatin1String("audio/wav");

    // AIFF
    types << QLatin1String("audio/x-aiff");

    // APE
    types << QLatin1String("audio/x-ape");

    // WV
    types << QLatin1String("audio/x-wavpack");

    return types;
}

Nepomuk2::SimpleResourceGraph TagLibExtractor::extract(const QUrl& resUri, const QUrl& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    TagLib::FileRef file(fileUrl.toLocalFile().toUtf8().data(), true);
    if (file.isNull()) {
        return SimpleResourceGraph();
    }

    SimpleResourceGraph graph;
    SimpleResource fileRes(resUri);

    TagLib::Tag* tags = file.tag();
    if (!tags->isEmpty()) {
        fileRes.addType(NMM::MusicPiece());
    } else {
        fileRes.addType(NFO::Audio());
    }

    TagLib::String artists;
    TagLib::String albumArtists;
    TagLib::String composers;
    TagLib::String lyricists;
    TagLib::StringList genres;

    // Handling multiple tags in mpeg files.
    if ((mimeType == "audio/mpeg") || (mimeType == "audio/mpeg3") || (mimeType == "audio/x-mpeg")) {
        TagLib::MPEG::File mpegFile(fileUrl.toLocalFile().toUtf8().data(), true);
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

    // Handling multiple tags in FLAC files.
    if (mimeType == "audio/flac") {
        TagLib::FLAC::File flacFile(fileUrl.toLocalFile().toUtf8().data(), true);
        if (flacFile.xiphComment() && !flacFile.xiphComment()->isEmpty()) {
            TagLib::Ogg::FieldListMap lstFLAC = flacFile.xiphComment()->fieldListMap();
            TagLib::Ogg::FieldListMap::ConstIterator itFLAC;

            // Artist.
            itFLAC = lstFLAC.find("ARTIST");
            if (itFLAC != lstFLAC.end()) {
                if (!artists.isEmpty()) {
                    artists += ", ";
                }
                artists += (*itFLAC).second.toString(", ");
            }

            // Album Artist.
            itFLAC = lstFLAC.find("ALBUMARTIST");
            if (itFLAC != lstFLAC.end()) {
                if (!albumArtists.isEmpty()) {
                    albumArtists += ", ";
                }
                albumArtists += (*itFLAC).second.toString(", ");
            }

            // Composer.
            itFLAC = lstFLAC.find("COMPOSER");
            if (itFLAC != lstFLAC.end()) {
                if (!composers.isEmpty()) {
                    composers += ", ";
                }
                composers += (*itFLAC).second.toString(", ");
            }

            // Lyricist.
            itFLAC = lstFLAC.find("LYRICIST");
            if (itFLAC != lstFLAC.end()) {
                if (!lyricists.isEmpty()) {
                    lyricists += ", ";
                }
                lyricists += (*itFLAC).second.toString(", ");
            }

            // Genre.
            itFLAC = lstFLAC.find("GENRE");
            if (itFLAC != lstFLAC.end()) {
                genres.append((*itFLAC).second);
            }
        }
    }

    // Handling multiple tags in OGG files.
    if (mimeType == "audio/ogg" || mimeType == "audio/x-vorbis+ogg") {
        TagLib::Ogg::Vorbis::File oggFile(fileUrl.toLocalFile().toUtf8().data(), true);
        if (oggFile.tag() && !oggFile.tag()->isEmpty()) {
            TagLib::Ogg::FieldListMap lstOGG = oggFile.tag()->fieldListMap();
            TagLib::Ogg::FieldListMap::ConstIterator itOGG;

            // Artist.
            itOGG = lstOGG.find("ARTIST");
            if (itOGG != lstOGG.end()) {
                if (!artists.isEmpty()) {
                    artists += ", ";
                }
                artists += (*itOGG).second.toString(", ");
            }

            // Album Artist.
            itOGG = lstOGG.find("ALBUMARTIST");
            if (itOGG != lstOGG.end()) {
                if (!albumArtists.isEmpty()) {
                    albumArtists += ", ";
                }
                albumArtists += (*itOGG).second.toString(", ");
            }

            // Composer.
            itOGG = lstOGG.find("COMPOSER");
            if (itOGG != lstOGG.end()) {
                if (!composers.isEmpty()) {
                    composers += ", ";
                }
                composers += (*itOGG).second.toString(", ");
            }

            // Lyricist.
            itOGG = lstOGG.find("LYRICIST");
            if (itOGG != lstOGG.end()) {
                if (!lyricists.isEmpty()) {
                    lyricists += ", ";
                }
                lyricists += (*itOGG).second.toString(", ");
            }

            // Genre.
            itOGG = lstOGG.find("GENRE");
            if (itOGG != lstOGG.end()) {
                genres.append((*itOGG).second);
            }
        }
    }

    if (!tags->isEmpty()) {
        QString title = QString::fromUtf8(tags->title().toCString(true));
        if (!title.isEmpty()) {
            fileRes.addProperty(NIE::title(), title);
        }

        QString comment = QString::fromUtf8(tags->comment().toCString(true));
        if (!comment.isEmpty()) {
            fileRes.addProperty(NIE::comment(), comment);
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

            fileRes.addProperty(NMM::genre(), genre);
        }

        QString artistString;
        if (artists.isEmpty()) {
            artistString = QString::fromUtf8(tags->artist().toCString(true));
        } else {
            artistString = QString::fromUtf8(artists.toCString(true)).trimmed();
        }

        QList< SimpleResource > artists = contactsFromString(artistString);
        foreach(const SimpleResource & res, artists) {
            fileRes.addProperty(NMM::performer(), res);
            graph << res;
        }

        QString composersString = QString::fromUtf8(composers.toCString(true)).trimmed();
        QList< SimpleResource > composers = contactsFromString(composersString);
        foreach(const SimpleResource & res, composers) {
            fileRes.addProperty(NMM::composer(), res);
            graph << res;
        }

        QString lyricistsString = QString::fromUtf8(lyricists.toCString(true)).trimmed();
        QList< SimpleResource > lyricists = contactsFromString(lyricistsString);
        foreach(const SimpleResource & res, lyricists) {
            fileRes.addProperty(NMM::lyricist(), res);
            graph << res;
        }

        QString album = QString::fromUtf8(tags->album().toCString(true));
        if (!album.isEmpty()) {
            SimpleResource albumRes;
            albumRes.addType(NMM::MusicAlbum());
            albumRes.setProperty(NIE::title(), album);

            QString albumArtistsString = QString::fromUtf8(albumArtists.toCString(true)).trimmed();
            QList< SimpleResource > albumArtists = contactsFromString(albumArtistsString);
            foreach(const SimpleResource & res, albumArtists) {
                albumRes.addProperty(NMM::albumArtist(), res);
                graph << res;
            }

            fileRes.setProperty(NMM::musicAlbum(), albumRes);
            graph << albumRes;
        }

        if (tags->track()) {
            fileRes.setProperty(NMM::trackNumber(), tags->track());
        }

        if (tags->year()) {
            QDateTime dt;
            dt.setUtcOffset(0); // Date must not be converted.
            QDate date = dt.date();
            // If there is only the year then January 1 it's used to complete the date.
            date.setDate(tags->year(), 1, 1);
            if (date.year() < 0) {
                date.setDate(1, 1, 1);
            }
            dt.setDate(date);
            fileRes.setProperty(NMM::releaseDate(), dt);
        }
    }

    TagLib::AudioProperties* audioProp = file.audioProperties();
    if (audioProp) {
        if (audioProp->length()) {
            // What about the xml duration?
            fileRes.setProperty(NFO::duration(), audioProp->length());
        }

        if (audioProp->bitrate()) {
            fileRes.setProperty(NFO::averageBitrate(), audioProp->bitrate() * 1000);
        }

        if (audioProp->channels()) {
            fileRes.setProperty(NFO::channels(), audioProp->channels());
        }

        if (audioProp->sampleRate()) {
            fileRes.setProperty(NFO::sampleRate(), audioProp->sampleRate());
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

    graph << fileRes;
    return graph;
}

}

NEPOMUK_EXPORT_EXTRACTOR(Nepomuk2::TagLibExtractor, "nepomuktaglibextextractor")
