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
#include <apefile.h>
#include <asftag.h>
#include <asfattribute.h>
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
#include <popularimeterframe.h>
#include <textidentificationframe.h>
#include <wavpackfile.h>

#include <QDateTime>
#include <QDebug>

using namespace KFileMetaData;

TagLibExtractor::TagLibExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{
}

const QStringList supportedMimeTypes = {
    QStringLiteral("audio/flac"),
    QStringLiteral("audio/mp4"),
    QStringLiteral("audio/mpeg"),
    QStringLiteral("audio/mpeg3"),
    QStringLiteral("audio/ogg"),
    QStringLiteral("audio/opus"),
    QStringLiteral("audio/x-aiff"),
    QStringLiteral("audio/x-ape"),
    QStringLiteral("audio/x-mpeg"),
    QStringLiteral("audio/x-ms-wma"),
    QStringLiteral("audio/x-musepack"),
    QStringLiteral("audio/x-opus+ogg"),
    QStringLiteral("audio/x-vorbis+ogg"),
    QStringLiteral("audio/wav"),
    QStringLiteral("audio/x-wavpack"),
};

QStringList TagLibExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void TagLibExtractor::extractId3Tags(TagLib::ID3v2::Tag* Id3Tags, ExtractedData& data)
{
    if (Id3Tags->isEmpty()) {
        return;
    }
    TagLib::ID3v2::FrameList lstID3v2;

    // Artist.
    lstID3v2 = Id3Tags->frameListMap()["TPE1"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.artists.isEmpty()) {
            data.artists += ", ";
        }
        data.artists += frame->toString();
    }

    // Album Artist.
    lstID3v2 = Id3Tags->frameListMap()["TPE2"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.albumArtists.isEmpty()) {
            data.albumArtists += ", ";
        }
        data.albumArtists += frame->toString();
    }

    // Composer.
    lstID3v2 = Id3Tags->frameListMap()["TCOM"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.composers.isEmpty()) {
            data.composers += ", ";
        }
        data.composers += frame->toString();
    }

    // Lyricist.
    lstID3v2 = Id3Tags->frameListMap()["TEXT"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.lyricists.isEmpty()) {
            data.lyricists += ", ";
        }
        data.lyricists += frame->toString();
    }

    // Genre.
    lstID3v2 = Id3Tags->frameListMap()["TCON"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        data.genres.append(frame->toString());
    }

    // Disc number.
    lstID3v2 = Id3Tags->frameListMap()["TPOS"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        data.discNumber = frame->toString().toInt();
    }

    // Performer.
    lstID3v2 = Id3Tags->frameListMap()["TMCL"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.performer.isEmpty()) {
            data.performer += ", ";
        }
        data.performer += frame->toString();
    }

    // Conductor.
    lstID3v2 = Id3Tags->frameListMap()["TPE3"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.conductor.isEmpty()) {
            data.conductor += ", ";
        }
        data.conductor += frame->toString();
    }

    // Publisher.
    lstID3v2 = Id3Tags->frameListMap()["TPUB"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.publisher.isEmpty()) {
            data.publisher += ", ";
        }
        data.publisher += frame->toString();
    }

    // Copyright.
    lstID3v2 = Id3Tags->frameListMap()["TCOP"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.copyright.isEmpty()) {
            data.copyright += ", ";
        }
        data.copyright += frame->toString();
    }

    // Language.
    lstID3v2 = Id3Tags->frameListMap()["TLAN"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.language.isEmpty()) {
            data.language += ", ";
        }
        data.language += frame->toString();
    }

    // Lyrics.
    lstID3v2 = Id3Tags->frameListMap()["USLT"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.lyrics.isEmpty()) {
            data.lyrics += ", ";
        }
        data.lyrics += frame->toString();
    }

    // Compilation.
    lstID3v2 = Id3Tags->frameListMap()["TCMP"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        if (!data.compilation.isEmpty()) {
            data.compilation += ", ";
        }
        data.compilation += frame->toString();
    }

    // Rating.
    /* There is no standard regarding ratings. Most of the implementations match
       a 5 stars rating to a range of 0-255 for MP3.
       Match it to baloo rating with a range of 0 - 10 */
    lstID3v2 = Id3Tags->frameListMap()["POPM"];
    for (const auto& frame : qAsConst(lstID3v2)) {
        TagLib::ID3v2::PopularimeterFrame *ratingFrame = static_cast<TagLib::ID3v2::PopularimeterFrame *>(frame);
        int rating = ratingFrame->rating();
        if (rating == 0) {
            data.rating = 0;
        } else if (rating == 1) {
            TagLib::String ratingProvider = ratingFrame->email();
            if (ratingProvider == "no@email" || ratingProvider == "org.kde.kfilemetadata") {
                data.rating = 1;
            } else {
                data.rating = 2;
            }
        } else if (rating >= 1 && rating <= 255) {
            data.rating = static_cast<int>(0.032 * rating + 2);
        }
    }

    // User Text Frame.
    lstID3v2 = Id3Tags->frameListMap()["TXXX"];
    if (!lstID3v2.isEmpty()) {
        // look for ReplayGain tags
        typedef TagLib::ID3v2::UserTextIdentificationFrame IdFrame;

        auto trackGainFrame = IdFrame::find(Id3Tags, "replaygain_track_gain");
        if (trackGainFrame && !trackGainFrame->fieldList().isEmpty()) {
            data.replayGainTrackGain = TStringToQString(trackGainFrame->fieldList().back());
        }

        auto trackPeakFrame = IdFrame::find(Id3Tags, "replaygain_track_peak");
        if (trackPeakFrame && !trackPeakFrame->fieldList().isEmpty()) {
            data.replayGainTrackPeak = TStringToQString(trackPeakFrame->fieldList().back());
        }

        auto albumGainFrame = IdFrame::find(Id3Tags, "replaygain_album_gain");
        if (albumGainFrame && !albumGainFrame->fieldList().isEmpty()) {
            data.replayGainAlbumGain = TStringToQString(albumGainFrame->fieldList().back());
        }

        auto albumPeakFrame = IdFrame::find(Id3Tags, "replaygain_album_peak");
        if (albumPeakFrame && !albumPeakFrame->fieldList().isEmpty()) {
            data.replayGainAlbumPeak = TStringToQString(albumPeakFrame->fieldList().back());
        }
    }
    //TODO handle TIPL tag
}

void TagLibExtractor::extractMp4Tags(TagLib::MP4::Tag* mp4Tags, ExtractedData& data)
{
    if (mp4Tags->isEmpty()) {
        return;
    }
    TagLib::MP4::ItemListMap allTags = mp4Tags->itemListMap();

    TagLib::MP4::ItemListMap::Iterator itAlbumArtists = allTags.find("aART");
    if (itAlbumArtists != allTags.end()) {
        data.albumArtists = itAlbumArtists->second.toStringList().toString(", ");
    }

    TagLib::MP4::ItemListMap::Iterator itDiscNumber = allTags.find("disk");
    if (itDiscNumber != allTags.end()) {
        data.discNumber = itDiscNumber->second.toInt();
    }

    TagLib::MP4::ItemListMap::Iterator itCompilation = allTags.find("cpil");
    if (itCompilation != allTags.end()) {
        data.compilation = itCompilation->second.toStringList().toString(", ");
    }

    TagLib::MP4::ItemListMap::Iterator itCopyright = allTags.find("cprt");
    if (itCopyright != allTags.end()) {
        data.copyright = itCopyright->second.toStringList().toString(", ");
    }

    TagLib::String genreAtomName(TagLib::String("©gen", TagLib::String::UTF8).to8Bit(), TagLib::String::Latin1);
    TagLib::MP4::ItemListMap::Iterator itGenres = allTags.find(genreAtomName);
    if (itGenres != allTags.end()) {
        data.genres = itGenres->second.toStringList().toString(", ");
    }

    TagLib::String composerAtomName(TagLib::String("©wrt", TagLib::String::UTF8).to8Bit(), TagLib::String::Latin1);
    TagLib::MP4::ItemListMap::Iterator itComposers = allTags.find(composerAtomName);
    if (itComposers != allTags.end()) {
        data.composers = itComposers->second.toStringList().toString(", ");
    }

    /* There is no standard regarding ratings. Mimic MediaMonkey's behavior
       with a range of 0 to 100 (stored in steps of 10) and make it compatible
       with baloo rating with a range from 0 to 10 */
    TagLib::MP4::ItemListMap::Iterator itRating = allTags.find("rate");
    if (itRating != allTags.end()) {
        data.rating = itRating->second.toStringList().toString().toInt() / 10;
    }

    TagLib::String lyricsAtomName(TagLib::String("©lyr", TagLib::String::UTF8).to8Bit(), TagLib::String::Latin1);
    TagLib::MP4::ItemListMap::Iterator itLyrics = allTags.find(lyricsAtomName);
    if (itLyrics != allTags.end()) {
        data.lyrics = itLyrics->second.toStringList().toString(", ");
    }
}

void TagLibExtractor::extractApeTags(TagLib::APE::Tag* apeTags, ExtractedData& data)
{
    if (apeTags->isEmpty()) {
        return;
    }
    TagLib::APE::ItemListMap lstMusepack = apeTags->itemListMap();
    TagLib::APE::ItemListMap::ConstIterator itMPC;

    itMPC = lstMusepack.find("ARTIST");
    if (itMPC != lstMusepack.end()) {
        if (!data.artists.isEmpty()) {
            data.artists += ", ";
        }
        data.artists += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("ALBUMARTIST");
    if (itMPC == lstMusepack.end()) {
        itMPC = lstMusepack.find("ALBUM ARTIST");
    }
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

    itMPC = lstMusepack.find("LOCATION");
    if (itMPC != lstMusepack.end()) {
        if (!data.location.isEmpty()) {
            data.location += ", ";
        }
        data.location += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("ARRANGER");
    if (itMPC != lstMusepack.end()) {
        if (!data.arranger.isEmpty()) {
            data.arranger += ", ";
        }
        data.arranger += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("PERFORMER");
    if (itMPC != lstMusepack.end()) {
        if (!data.performer.isEmpty()) {
            data.performer += ", ";
        }
        data.performer += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("CONDUCTOR");
    if (itMPC != lstMusepack.end()) {
        if (!data.conductor.isEmpty()) {
            data.conductor += ", ";
        }
        data.conductor += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("ENSEMBLE");
    if (itMPC != lstMusepack.end()) {
        if (!data.ensemble.isEmpty()) {
            data.ensemble += ", ";
        }
        data.ensemble += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("PUBLISHER");
    if (itMPC != lstMusepack.end()) {
        if (!data.publisher.isEmpty()) {
            data.publisher += ", ";
        }
        data.publisher += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("COPYRIGHT");
    if (itMPC != lstMusepack.end()) {
        if (!data.copyright.isEmpty()) {
            data.copyright += ", ";
        }
        data.copyright += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("LABEL");
    if (itMPC != lstMusepack.end()) {
        if (!data.label.isEmpty()) {
            data.label += ", ";
        }
        data.label += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("AUTHOR");
    if (itMPC != lstMusepack.end()) {
        if (!data.author.isEmpty()) {
            data.author += ", ";
        }
        data.author += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("LICENSE");
    if (itMPC != lstMusepack.end()) {
        if (!data.license.isEmpty()) {
            data.license += ", ";
        }
        data.license += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("LYRICS");
    if (itMPC != lstMusepack.end()) {
        if (!data.lyrics.isEmpty()) {
            data.lyrics += ", ";
        }
        data.lyrics += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("COMPILATION");
    if (itMPC != lstMusepack.end()) {
        if (!data.compilation.isEmpty()) {
            data.compilation += ", ";
        }
        data.compilation += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("LANGUAGE");
    if (itMPC != lstMusepack.end()) {
        if (!data.language.isEmpty()) {
            data.language += ", ";
        }
        data.language += (*itMPC).second.toString();
    }

    itMPC = lstMusepack.find("DISCNUMBER");
    if (itMPC != lstMusepack.end()) {
        data.discNumber = (*itMPC).second.toString().toInt();
    }

    itMPC = lstMusepack.find("OPUS");
    if (itMPC != lstMusepack.end()) {
        data.opus = (*itMPC).second.toString().toInt();
    }

    // Rating.
    itMPC = lstMusepack.find("RATING");
    if (itMPC != lstMusepack.end()) {
        /* There is no standard regarding ratings. There is one implementation
           most seem to follow with a range of 0 to 100 (stored in steps of 10).
           Make it compatible with baloo rating with a range from 0 to 10 */
        data.rating = (*itMPC).second.toString().toInt() / 10;
    }

    itMPC = lstMusepack.find("REPLAYGAIN_TRACK_GAIN");
    if (itMPC != lstMusepack.end()) {
        data.replayGainTrackGain = TStringToQString((*itMPC).second.toString());
    }

    itMPC = lstMusepack.find("REPLAYGAIN_TRACK_PEAK");
    if (itMPC != lstMusepack.end()) {
        data.replayGainTrackPeak = TStringToQString((*itMPC).second.toString());
    }

    itMPC = lstMusepack.find("REPLAYGAIN_ALBUM_GAIN");
    if (itMPC != lstMusepack.end()) {
        data.replayGainAlbumGain = TStringToQString((*itMPC).second.toString());
    }

    itMPC = lstMusepack.find("REPLAYGAIN_ALBUM_PEAK");
    if (itMPC != lstMusepack.end()) {
        data.replayGainAlbumPeak = TStringToQString((*itMPC).second.toString());
    }
}

void TagLibExtractor::extractVorbisTags(TagLib::Ogg::XiphComment* vorbisTags, ExtractedData& data)
{
    if (vorbisTags->isEmpty()) {
        return;
    }
    TagLib::Ogg::FieldListMap lstOgg = vorbisTags->fieldListMap();
    TagLib::Ogg::FieldListMap::ConstIterator itOgg;

    itOgg = lstOgg.find("ARTIST");
    if (itOgg != lstOgg.end()) {
        if (!data.artists.isEmpty()) {
            data.artists += ", ";
        }
        data.artists += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("ALBUMARTIST");
    if (itOgg == lstOgg.end())  {
        itOgg = lstOgg.find("ALBUM ARTIST");
    }
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

    itOgg = lstOgg.find("LOCATION");
    if (itOgg != lstOgg.end()) {
        if (!data.location.isEmpty()) {
            data.location += ", ";
        }
        data.location += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("ARRANGER");
    if (itOgg != lstOgg.end()) {
        if (!data.arranger.isEmpty()) {
            data.arranger += ", ";
        }
        data.arranger += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("PERFORMER");
    if (itOgg != lstOgg.end()) {
        if (!data.performer.isEmpty()) {
            data.performer += ", ";
        }
        data.performer += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("CONDUCTOR");
    if (itOgg != lstOgg.end()) {
        if (!data.conductor.isEmpty()) {
            data.conductor += ", ";
        }
        data.conductor += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("ENSEMBLE");
    if (itOgg != lstOgg.end()) {
        if (!data.ensemble.isEmpty()) {
            data.ensemble += ", ";
        }
        data.ensemble += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("PUBLISHER");
    if (itOgg != lstOgg.end()) {
        if (!data.publisher.isEmpty()) {
            data.publisher += ", ";
        }
        data.publisher += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("COPYRIGHT");
    if (itOgg != lstOgg.end()) {
        if (!data.copyright.isEmpty()) {
            data.copyright += ", ";
        }
        data.copyright += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("LABEL");
    if (itOgg != lstOgg.end()) {
        if (!data.label.isEmpty()) {
            data.label += ", ";
        }
        data.label += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("AUTHOR");
    if (itOgg != lstOgg.end()) {
        if (!data.author.isEmpty()) {
            data.author += ", ";
        }
        data.author += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("LICENSE");
    if (itOgg != lstOgg.end()) {
        if (!data.license.isEmpty()) {
            data.license += ", ";
        }
        data.license += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("LYRICS");
    if (itOgg != lstOgg.end()) {
        if (!data.lyrics.isEmpty()) {
            data.lyrics += ", ";
        }
        data.lyrics += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("COMPILATION");
    if (itOgg != lstOgg.end()) {
        if (!data.compilation.isEmpty()) {
            data.compilation += ", ";
        }
        data.compilation += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("LANGUAGE");
    if (itOgg != lstOgg.end()) {
        if (!data.language.isEmpty()) {
            data.language += ", ";
        }
        data.language += (*itOgg).second.toString(", ");
    }

    itOgg = lstOgg.find("GENRE");
    if (itOgg != lstOgg.end()) {
        data.genres.append((*itOgg).second);
    }

    itOgg = lstOgg.find("DISCNUMBER");
    if (itOgg != lstOgg.end()) {
        data.discNumber = (*itOgg).second.toString("").toInt();
    }

    itOgg = lstOgg.find("OPUS");
    if (itOgg != lstOgg.end()) {
        data.opus = (*itOgg).second.toString("").toInt();
    }

    itOgg = lstOgg.find("RATING");
    if (itOgg != lstOgg.end()) {
        //there is no standard regarding ratings. There is one implementation
        //most seem to follow with a range of 0 to 100 (stored in steps of 10).
        //make it compatible with baloo rating with a range from 0 to 10
        data.rating = (*itOgg).second.toString("").toInt() / 10;
    }

    itOgg = lstOgg.find("REPLAYGAIN_TRACK_GAIN");
    if (itOgg != lstOgg.end()) {
        data.replayGainTrackGain = TStringToQString((*itOgg).second.toString(""));
    }

    itOgg = lstOgg.find("REPLAYGAIN_TRACK_PEAK");
    if (itOgg != lstOgg.end()) {
        data.replayGainTrackPeak = TStringToQString((*itOgg).second.toString(""));
    }

    itOgg = lstOgg.find("REPLAYGAIN_ALBUM_GAIN");
    if (itOgg != lstOgg.end()) {
        data.replayGainAlbumGain = TStringToQString((*itOgg).second.toString(""));
    }

    itOgg = lstOgg.find("REPLAYGAIN_ALBUM_PEAK");
    if (itOgg != lstOgg.end()) {
        data.replayGainAlbumPeak = TStringToQString((*itOgg).second.toString(""));
    }
}

void TagLibExtractor::extractAsfTags(TagLib::ASF::Tag* asfTags, ExtractedData& data)
{
    if (asfTags->isEmpty()) {
        return;
    }

    if (!asfTags->copyright().isEmpty()) {
        data.copyright = asfTags->copyright();
    }

    TagLib::ASF::AttributeList lstASF = asfTags->attribute("WM/SharedUserRating");
    if (!lstASF.isEmpty()) {
        int rating = lstASF.front().toString().toInt();
        //map the rating values of WMP to Baloo rating
        //0->0, 1->2, 25->4, 50->6, 75->8, 99->10
        if (rating == 0) {
            data.rating = 0;
        } else if (rating == 1) {
            data.rating = 2;
        } else {
            data.rating = static_cast<uint>(0.09 * rating + 2);
        }
    }

    lstASF = asfTags->attribute("WM/PartOfSet");
    if (!lstASF.isEmpty()) {
        data.discNumber = lstASF.front().toString().toInt();
    }

    lstASF = asfTags->attribute("WM/AlbumArtist");
    for (const auto& attribute : qAsConst(lstASF)) {
        if (!data.albumArtists.isEmpty()) {
            data.albumArtists += ", ";
        }
        data.albumArtists += attribute.toString();
    }

    lstASF = asfTags->attribute("WM/Composer");
    for (const auto& attribute : qAsConst(lstASF)) {
        if (!data.composers.isEmpty()) {
            data.composers += ", ";
        }
        data.composers += attribute.toString();
    }

    lstASF = asfTags->attribute("WM/Conductor");
    for (const auto& attribute : qAsConst(lstASF)) {
        if (!data.conductor.isEmpty()) {
            data.conductor += ", ";
        }
        data.conductor += attribute.toString();
    }

    lstASF = asfTags->attribute("WM/Writer");
    for (const auto& attribute : qAsConst(lstASF)) {
        if (!data.lyricists.isEmpty()) {
            data.lyricists += ", ";
        }
        data.lyricists += attribute.toString();
    }

    lstASF = asfTags->attribute("WM/Publisher");
    for (const auto& attribute : qAsConst(lstASF)) {
        if (!data.publisher.isEmpty()) {
            data.publisher += ", ";
        }
        data.publisher += attribute.toString();
    }

    lstASF = asfTags->attribute("Author");
    for (const auto& attribute : qAsConst(lstASF)) {
        if (!data.author.isEmpty()) {
            data.author += ", ";
        }
        data.author += attribute.toString();
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

    TagLib::FileRef file(&stream, true);
    if (file.isNull()) {
        qWarning() << "Unable to open file: " << fileUrl;
        return;
    }

    TagLib::Tag* tags = file.tag();
    result->addType(Type::Audio);

    ExtractedData data;

    if ((mimeType == QLatin1String("audio/mpeg")) || (mimeType == QLatin1String("audio/mpeg3"))
            || (mimeType == QLatin1String("audio/x-mpeg"))) {
        TagLib::MPEG::File mpegFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        if (mpegFile.hasID3v2Tag()) {
            extractId3Tags(mpegFile.ID3v2Tag(), data);
        }
    } else if ((mimeType == QLatin1String("audio/x-aiff")) || (mimeType == QLatin1String("audio/wav"))) {
        /* For some reason, TagLib::RIFF::AIFF::File and TagLib::RIFF::WAV::File tag() return
         * only an invalid pointer. Use the dynamic_cast instead. */
        TagLib::ID3v2::Tag* ID3v2Tag = dynamic_cast<TagLib::ID3v2::Tag*>(tags);
        if (ID3v2Tag) {
            extractId3Tags(ID3v2Tag, data);
        }
    } else if (mimeType == QLatin1String("audio/mp4")) {
        TagLib::MP4::File mp4File(&stream, true);
        if (mp4File.hasMP4Tag()) {
            extractMp4Tags(mp4File.tag(), data);
        }
    } else if (mimeType == QLatin1String("audio/x-musepack")) {
        TagLib::MPC::File mpcFile(&stream, true);
        if (mpcFile.hasAPETag()) {
            extractApeTags(mpcFile.APETag(), data);
        }
    } else if (mimeType == QLatin1String("audio/x-ape")) {
        TagLib::APE::File apeFile(&stream, true);
        if (apeFile.hasAPETag()) {
            extractApeTags(apeFile.APETag(), data);
        }
    } else if (mimeType == QLatin1String("audio/x-wavpack")) {
        TagLib::WavPack::File wavpackFile(&stream, true);
        if (wavpackFile.hasAPETag()) {
            extractApeTags(wavpackFile.APETag(), data);
        }
    } else if (mimeType == QLatin1String("audio/flac")) {
        TagLib::FLAC::File flacFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        if (flacFile.hasXiphComment()) {
            extractVorbisTags(flacFile.xiphComment(), data);
        }
    } else if (mimeType == QLatin1String("audio/ogg") || mimeType == QLatin1String("audio/x-vorbis+ogg")) {
        TagLib::Ogg::Vorbis::File oggFile(&stream, true);
        if (oggFile.tag()) {
            extractVorbisTags(oggFile.tag(), data);
        }
    } else if (mimeType == QLatin1String("audio/opus") || mimeType == QLatin1String("audio/x-opus+ogg")) {
        TagLib::Ogg::Opus::File opusFile(&stream, true);
        if (opusFile.tag()) {
            extractVorbisTags(opusFile.tag(), data);
        }
    } else if (mimeType == QLatin1String("audio/x-ms-wma")) {
        /* For some reason, TagLib::ASF::File tag() returns only an invalid pointer.
         * Use the dynamic_cast instead. */
        TagLib::ASF::Tag* asfTags = dynamic_cast<TagLib::ASF::Tag*>(tags);
        if (asfTags) {
            extractAsfTags(asfTags, data);
        }
    }

    if (!tags->isEmpty()) {
        QString title = TStringToQString(tags->title());
        if (!title.isEmpty()) {
            result->add(Property::Title, title);
        }

        QString comment = TStringToQString(tags->comment());
        if (!comment.isEmpty()) {
            result->add(Property::Comment, comment);
        }

        if (data.genres.isEmpty()) {
            data.genres.append(tags->genre());
        }

        for (uint i = 0; i < data.genres.size(); i++) {
            QString genre = TStringToQString(data.genres[i]).trimmed();
            if (!genre.isEmpty()) {
                // Convert from int
                bool ok = false;
                int genreNum = genre.toInt(&ok);
                if (ok) {
                    genre = TStringToQString(TagLib::ID3v1::genre(genreNum));
                }
                result->add(Property::Genre, genre);
            }
        }

        const auto artistString = data.artists.isEmpty()
                ? TStringToQString(tags->artist())
                : TStringToQString(data.artists).trimmed();
        const auto artists = contactsFromString(artistString);
        for (auto& artist : artists) {
            result->add(Property::Artist, artist);
        }

        const auto  composersString = TStringToQString(data.composers).trimmed();
        const auto composers = contactsFromString(composersString);
        for (auto& comp : composers) {
            result->add(Property::Composer, comp);
        }

        const auto lyricistsString = TStringToQString(data.lyricists).trimmed();
        const auto lyricists = contactsFromString(lyricistsString);
        for (auto& lyr : lyricists) {
            result->add(Property::Lyricist, lyr);
        }

        const auto album = TStringToQString(tags->album());
        if (!album.isEmpty()) {
            result->add(Property::Album, album);

            const auto albumArtistsString = TStringToQString(data.albumArtists).trimmed();
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

        QString locationsString = TStringToQString(data.location).trimmed();
        QStringList locations = contactsFromString(locationsString);
        foreach(const QString& loc, locations) {
            result->add(Property::Location, loc);
        }

        QString performersString = TStringToQString(data.performer).trimmed();
        QStringList performers = contactsFromString(performersString);
        foreach(const QString& per, performers) {
            result->add(Property::Performer, per);
        }

        QString ensembleString = TStringToQString(data.ensemble).trimmed();
        QStringList ensembles = contactsFromString(ensembleString);
        foreach(const QString& ens, ensembles) {
            result->add(Property::Ensemble, ens);
        }

        QString arrangerString = TStringToQString(data.arranger).trimmed();
        QStringList arrangers = contactsFromString(arrangerString);
        foreach(const QString& arr, arrangers) {
            result->add(Property::Arranger, arr);
        }

        QString conductorString = TStringToQString(data.conductor).trimmed();
        QStringList conductors = contactsFromString(conductorString);
        foreach(const QString& arr, conductors) {
            result->add(Property::Conductor, arr);
        }

        QString publisherString = TStringToQString(data.publisher).trimmed();
        QStringList publishers = contactsFromString(publisherString);
        foreach(const QString& arr, publishers) {
            result->add(Property::Publisher, arr);
        }

        QString copyrightString = TStringToQString(data.copyright).trimmed();
        QStringList copyrights = contactsFromString(copyrightString);
        foreach(const QString& arr, copyrights) {
            result->add(Property::Copyright, arr);
        }

        QString labelString = TStringToQString(data.label).trimmed();
        QStringList labels = contactsFromString(labelString);
        foreach(const QString& arr, labels) {
            result->add(Property::Label, arr);
        }

        QString authorString = TStringToQString(data.author).trimmed();
        QStringList authors = contactsFromString(authorString);
        foreach(const QString& arr, authors) {
            result->add(Property::Author, arr);
        }

        QString languageString = TStringToQString(data.language).trimmed();
        QStringList languages = contactsFromString(languageString);
        foreach(const QString& arr, languages) {
            result->add(Property::Language, arr);
        }

        QString licenseString = TStringToQString(data.license).trimmed();
        QStringList licenses = contactsFromString(licenseString);
        foreach(const QString& arr, licenses) {
            result->add(Property::License, arr);
        }

        QString compilationString = TStringToQString(data.compilation).trimmed();
        QStringList compilations = contactsFromString(compilationString);
        foreach(const QString& arr, compilations) {
            result->add(Property::Compilation, arr);
        }

        QString lyricsString = TStringToQString(data.lyrics).trimmed();
        if (!lyricsString.isEmpty()) {
            result->add(Property::Lyrics, lyricsString);
        }

        if (data.opus.isValid()) {
            result->add(Property::Opus, data.opus);
        }

        if (data.discNumber.isValid()) {
            result->add(Property::DiscNumber, data.discNumber);
        }

        if (data.rating.isValid()) {
            result->add(Property::Rating, data.rating);
        }

        if (!data.replayGainAlbumGain.isEmpty()) {
            /* remove " dB" suffix */
            if (data.replayGainAlbumGain.endsWith(QStringLiteral(" dB"), Qt::CaseInsensitive))
            {
                data.replayGainAlbumGain.chop(3);
            }
            bool success = false;
            double replayGainAlbumGain = data.replayGainAlbumGain.toDouble(&success);
            if (success) {
                result->add(Property::ReplayGainAlbumGain, replayGainAlbumGain);
            }
        }

        if (!data.replayGainAlbumPeak.isEmpty()) {
            bool success = false;
            double replayGainAlbumPeak = data.replayGainAlbumPeak.toDouble(&success);
            if (success) {
                result->add(Property::ReplayGainAlbumPeak, replayGainAlbumPeak);
            }
        }

        if (!data.replayGainTrackGain.isEmpty()) {
            /* remove " dB" suffix */
            if (data.replayGainTrackGain.endsWith(QStringLiteral(" dB"), Qt::CaseInsensitive))
            {
                data.replayGainTrackGain.chop(3);
            }
            bool success = false;
            double replayGainTrackGain = data.replayGainTrackGain.toDouble(&success);
            if (success) {
                result->add(Property::ReplayGainTrackGain, replayGainTrackGain);
            }
        }

        if (!data.replayGainTrackPeak.isEmpty()) {
            bool success = false;
            double replayGainTrackPeak = data.replayGainTrackPeak.toDouble(&success);
            if (success) {
                result->add(Property::ReplayGainTrackPeak, replayGainTrackPeak);
            }
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
}

// TAG information (incomplete).
// https://xiph.org/vorbis/doc/v-comment.html
// http://help.mp3tag.de/main_tags.html
// http://id3.org/
// https://www.legroom.net/2009/05/09/ogg-vorbis-and-flac-comment-field-recommendations
// https://kodi.wiki/view/Music_tagging#Tags_Kodi_reads
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
