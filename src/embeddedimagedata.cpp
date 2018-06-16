/*
 * Copyright (C) 2018  Alexander Stippich <a.stippich@gmx.net>
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

#include "embeddedimagedata.h"
// Taglib includes
#include <taglib.h>
#include <tag.h>
#include <tfilestream.h>
#include <mpegfile.h>
#include <oggfile.h>
#include <mp4file.h>
#include <flacfile.h>
#include <vorbisfile.h>
#include <opusfile.h>
#include <apetag.h>
#include <mpcfile.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mp4tag.h>
#include <xiphcomment.h>

#include <QMimeDatabase>
#include <QDebug>

using namespace KFileMetaData;

class Q_DECL_HIDDEN EmbeddedImageData::Private
{
public:
    QMimeDatabase mMimeDatabase;
    QByteArray getFrontCover(const QString &fileUrl, const QString &mimeType) const;

};

EmbeddedImageData::EmbeddedImageData()
    : d(std::unique_ptr<Private>(new Private()))
{
}

EmbeddedImageData::~EmbeddedImageData()
= default;

QMap<EmbeddedImageData::ImageType, QByteArray>
EmbeddedImageData::imageData(const QString &fileUrl,
                             const EmbeddedImageData::ImageTypes types) const
{
    QMap<EmbeddedImageData::ImageType, QByteArray> imageData;

    const auto &fileMimeType = d->mMimeDatabase.mimeTypeForFile(fileUrl);
    if (fileMimeType.name().startsWith(QStringLiteral("audio/"))) {
        if (types & EmbeddedImageData::FrontCover) {
            imageData.insert(EmbeddedImageData::FrontCover, d->getFrontCover(fileUrl, fileMimeType.name()));
        }
    }

    return imageData;
}

QByteArray
EmbeddedImageData::Private::getFrontCover(const QString &fileUrl,
                                          const QString &mimeType) const
{
    TagLib::FileStream stream(fileUrl.toUtf8().constData(), true);
    if (!stream.isOpen()) {
        qWarning() << "Unable to open file readonly: " << fileUrl;
        return QByteArray();
    }
    if ((mimeType == QStringLiteral("audio/mpeg"))
            || (mimeType == QStringLiteral("audio/mpeg3"))
            || (mimeType == QStringLiteral("audio/x-mpeg"))) {

        // Handling multiple tags in mpeg files.
        TagLib::MPEG::File mpegFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        if (!mpegFile.ID3v2Tag() || mpegFile.ID3v2Tag()->isEmpty()) {
            return QByteArray();
        }

        TagLib::ID3v2::FrameList lstID3v2;
        // Attached Front Picture.
        lstID3v2 = mpegFile.ID3v2Tag()->frameListMap()["APIC"];
        if (!lstID3v2.isEmpty()) {
            for (TagLib::ID3v2::FrameList::ConstIterator it = lstID3v2.begin(); it != lstID3v2.end(); ++it) {
                auto *frontCoverFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(*it);
                if (frontCoverFrame->type() == frontCoverFrame->FrontCover) {
                    return QByteArray(frontCoverFrame->picture().data(), frontCoverFrame->picture().size());
                }
            }
        }

    } else if (mimeType == QStringLiteral("audio/mp4")) {

        TagLib::MP4::File mp4File(&stream, true);
        if (!mp4File.tag() || mp4File.tag()->isEmpty()) {
            return QByteArray();
        }
        TagLib::MP4::Item coverArtItem = mp4File.tag()->item("covr");
        if (coverArtItem.isValid())
        {
            TagLib::MP4::CoverArtList coverArtList = coverArtItem.toCoverArtList();
            TagLib::MP4::CoverArt& frontCover = coverArtList.front();
            return QByteArray(frontCover.data().data(), frontCover.data().size());
        }

    } else if (mimeType == QStringLiteral("audio/x-musepack")) {

        TagLib::MPC::File mpcFile(&stream, true);
        if (!mpcFile.tag() || mpcFile.tag()->isEmpty()) {
            return QByteArray();
        }

        TagLib::APE::ItemListMap lstMusepack = mpcFile.APETag()->itemListMap();
        TagLib::APE::ItemListMap::ConstIterator itMPC;

        /* The cover art tag for APEv2 tags starts with the filename as string
         * with zero termination followed by the actual picture data */
        itMPC = lstMusepack.find("COVER ART (FRONT)");
        if (itMPC != lstMusepack.end()) {
            TagLib::ByteVector pictureData = (*itMPC).second.binaryData();
            int dataPosition = pictureData.find('\0') + 1;
            return QByteArray(pictureData.data() + dataPosition, pictureData.size() - dataPosition);
        }

    } else if (mimeType == QStringLiteral("audio/flac")) {

        TagLib::FLAC::File flacFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        TagLib::List<TagLib::FLAC::Picture *> lstPic = flacFile.pictureList();

        if (!lstPic.isEmpty()) {
            for (TagLib::List<TagLib::FLAC::Picture *>::Iterator it = lstPic.begin(); it != lstPic.end(); ++it) {
                TagLib::FLAC::Picture *picture = *it;
                if (picture->type() == picture->FrontCover) {
                    return QByteArray(picture->data().data(), picture->data().size());
                }
            }
        }

    } else {

        TagLib::List<TagLib::FLAC::Picture *> lstPic;
        if (mimeType == QStringLiteral("audio/ogg") || mimeType == QStringLiteral("audio/x-vorbis+ogg")) {
            TagLib::Ogg::Vorbis::File oggFile(&stream, true);
            if (oggFile.tag() && !oggFile.tag()->isEmpty()) {
                lstPic = oggFile.tag()->pictureList();
            }
        }
        if (mimeType == QStringLiteral("audio/opus") || mimeType == QStringLiteral("audio/x-opus+ogg")) {
            TagLib::Ogg::Opus::File opusFile(&stream, true);
            if (opusFile.tag() && !opusFile.tag()->isEmpty()) {
                lstPic = opusFile.tag()->pictureList();
            }
        }
        if (!lstPic.isEmpty()) {
            for (TagLib::List<TagLib::FLAC::Picture *>::Iterator it = lstPic.begin(); it != lstPic.end(); ++it) {
                TagLib::FLAC::Picture *picture = *it;
                if (picture->type() == picture->FrontCover) {
                    return QByteArray(picture->data().data(), picture->data().size());
                }
            }
        }
    }
    return QByteArray();
}
