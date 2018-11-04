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


#ifndef TAGLIBEXTRACTOR_H
#define TAGLIBEXTRACTOR_H

#include "extractorplugin.h"
#include <tstring.h>
#include <tstringlist.h>

namespace TagLib
{
    namespace ASF {
        class Tag;
    }
    namespace ID3v2 {
        class Tag;
    }
    namespace MP4 {
        class Tag;
    }
    namespace APE {
        class Tag;
    }
    namespace Ogg {
        class XiphComment;
    }
}

namespace KFileMetaData
{

class TagLibExtractor : public ExtractorPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.kde.kf5.kfilemetadata.ExtractorPlugin"
                      FILE "taglibextractor.json")
    Q_INTERFACES(KFileMetaData::ExtractorPlugin)

public:
    explicit TagLibExtractor(QObject* parent = nullptr);

    void extract(ExtractionResult* result) override;
    QStringList mimetypes() const override;
    
private:
    struct ExtractedData {
        TagLib::String artists;
        TagLib::String albumArtists;
        TagLib::String composers;
        TagLib::String lyricists;
        TagLib::String location;
        TagLib::String performer;
        TagLib::String conductor;
        TagLib::String copyright;
        TagLib::String ensemble;
        TagLib::String arranger;
        TagLib::String language;
        TagLib::String publisher;
        TagLib::String label;
        TagLib::String author;
        TagLib::String license;
        TagLib::String lyrics;
        TagLib::String compilation;
        TagLib::StringList genres;
        QString replayGainAlbumGain;
        QString replayGainAlbumPeak;
        QString replayGainTrackGain;
        QString replayGainTrackPeak;
        QVariant discNumber;
        QVariant opus;
        QVariant rating;
    };
    void extractId3Tags(TagLib::ID3v2::Tag* id3Tags, ExtractedData& data);
    void extractMp4Tags(TagLib::MP4::Tag* mp4Tags, ExtractedData& data);
    void extractApeTags(TagLib::APE::Tag* apeTags, ExtractedData& data);
    void extractVorbisTags(TagLib::Ogg::XiphComment* vorbisTags, ExtractedData& data);
    void extractAsfTags(TagLib::ASF::Tag* asfTags, ExtractedData& data);
};

}

#endif // TAGLIBEXTRACTOR_H
