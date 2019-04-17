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
#include <mpcfile.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mp4tag.h>
#include <xiphcomment.h>
#include <apefile.h>
#include <apetag.h>
#include <wavpackfile.h>
#include <speexfile.h>
#include <wavfile.h>
#include <aifffile.h>
#include <asffile.h>
#include <asfattribute.h>
#include <asfpicture.h>

#include <QMimeDatabase>
#include <QDebug>

using namespace KFileMetaData;

class Q_DECL_HIDDEN EmbeddedImageData::Private
{
public:
    QMimeDatabase mMimeDatabase;
    QByteArray getFrontCover(const QString &fileUrl, const QString &mimeType) const;
    QByteArray getFrontCoverFromID3(TagLib::ID3v2::Tag* Id3Tags) const;
    QByteArray getFrontCoverFromFlacPicture(TagLib::List<TagLib::FLAC::Picture *> lstPic) const;
    QByteArray getFrontCoverFromMp4(TagLib::MP4::Tag* mp4Tags) const;
    QByteArray getFrontCoverFromApe(TagLib::APE::Tag* apeTags) const;
    QByteArray getFrontCoverFromAsf(TagLib::ASF::Tag* asfTags) const;
    static const QStringList mMimetypes;
};

const QStringList EmbeddedImageData::Private::mMimetypes =
{
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
    QStringLiteral("audio/x-speex"),
    QStringLiteral("audio/x-vorbis+ogg"),
    QStringLiteral("audio/x-wav"),
    QStringLiteral("audio/x-wavpack"),
};

EmbeddedImageData::EmbeddedImageData()
    : d(std::unique_ptr<Private>(new Private()))
{
}

EmbeddedImageData::~EmbeddedImageData()
= default;

QStringList EmbeddedImageData::mimeTypes() const
{
    return d->mMimetypes;
}

QMap<EmbeddedImageData::ImageType, QByteArray>
EmbeddedImageData::imageData(const QString &fileUrl,
                             const EmbeddedImageData::ImageTypes types) const
{
    QMap<EmbeddedImageData::ImageType, QByteArray> imageData;

    const auto &fileMimeType = d->mMimeDatabase.mimeTypeForFile(fileUrl);
    if (fileMimeType.name().startsWith(QLatin1String("audio/"))) {
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
    if ((mimeType == QLatin1String("audio/mpeg"))
            || (mimeType == QLatin1String("audio/mpeg3"))
            || (mimeType == QLatin1String("audio/x-mpeg"))) {

        // Handling multiple tags in mpeg files.
        TagLib::MPEG::File mpegFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        if (mpegFile.ID3v2Tag()) {
            return getFrontCoverFromID3(mpegFile.ID3v2Tag());
        }

    } else if (mimeType == QLatin1String("audio/x-aiff")) {

        TagLib::RIFF::AIFF::File aiffFile(&stream, true);
        if (aiffFile.hasID3v2Tag()) {
            return getFrontCoverFromID3(aiffFile.tag());
        }

    } else if ((mimeType == QLatin1String("audio/wav"))
               || (mimeType == QLatin1String("audio/x-wav"))) {

        TagLib::RIFF::WAV::File wavFile(&stream, true);
        if (wavFile.hasID3v2Tag()) {
            return getFrontCoverFromID3(wavFile.ID3v2Tag());
        }

    } else if (mimeType == QLatin1String("audio/mp4")) {

        TagLib::MP4::File mp4File(&stream, true);
        if (mp4File.tag()) {
            return getFrontCoverFromMp4(mp4File.tag());
        }

    } else if (mimeType == QLatin1String("audio/x-musepack")) {

        TagLib::MPC::File mpcFile(&stream, true);
        if (mpcFile.APETag()) {
            return getFrontCoverFromApe(mpcFile.APETag());
        }

    } else if (mimeType == QLatin1String("audio/x-ape")) {

        TagLib::APE::File apeFile(&stream, true);
        if (apeFile.hasAPETag()) {
            return getFrontCoverFromApe(apeFile.APETag());
        }

    } else if (mimeType == QLatin1String("audio/x-wavpack")) {

        TagLib::WavPack::File wavpackFile(&stream, true);
        if (wavpackFile.hasAPETag()) {
            return getFrontCoverFromApe(wavpackFile.APETag());
        }

    } else if (mimeType == QLatin1String("audio/x-ms-wma")) {

        TagLib::ASF::File asfFile(&stream, true);
        TagLib::ASF::Tag* asfTags = dynamic_cast<TagLib::ASF::Tag*>(asfFile.tag());
        if (asfTags) {
            return getFrontCoverFromAsf(asfTags);
        }

    } else if (mimeType == QLatin1String("audio/flac")) {

        TagLib::FLAC::File flacFile(&stream, TagLib::ID3v2::FrameFactory::instance(), true);
        return getFrontCoverFromFlacPicture(flacFile.pictureList());

    } else if ((mimeType == QLatin1String("audio/ogg"))
               || (mimeType == QLatin1String("audio/x-vorbis+ogg"))) {

        TagLib::Ogg::Vorbis::File oggFile(&stream, true);
        if (oggFile.tag()) {
            return getFrontCoverFromFlacPicture(oggFile.tag()->pictureList());
        }

    }
    else if ((mimeType == QLatin1String("audio/opus"))
             || (mimeType == QLatin1String("audio/x-opus+ogg"))) {

        TagLib::Ogg::Opus::File opusFile(&stream, true);
        if (opusFile.tag()) {
            return getFrontCoverFromFlacPicture(opusFile.tag()->pictureList());
        }

    } else if (mimeType == QLatin1String("audio/speex") || mimeType == QLatin1String("audio/x-speex")) {

        TagLib::Ogg::Speex::File speexFile(&stream, true);
        if (speexFile.tag()) {
            return getFrontCoverFromFlacPicture(speexFile.tag()->pictureList());
        }

    }
    return QByteArray();
}

QByteArray
EmbeddedImageData::Private::getFrontCoverFromID3(TagLib::ID3v2::Tag* Id3Tags) const
{
    TagLib::ID3v2::FrameList lstID3v2;
    // Attached Front Picture.
    lstID3v2 = Id3Tags->frameListMap()["APIC"];
    for (const auto& frame : qAsConst(lstID3v2))
    {
        const auto *frontCoverFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
        if (frontCoverFrame->type() == frontCoverFrame->FrontCover) {
            return QByteArray(frontCoverFrame->picture().data(), frontCoverFrame->picture().size());
        }
    }
    return QByteArray();
}

QByteArray
EmbeddedImageData::Private::getFrontCoverFromFlacPicture(TagLib::List<TagLib::FLAC::Picture *> lstPic) const
{
    for (const auto &picture : qAsConst(lstPic)) {
        if (picture->type() == picture->FrontCover) {
            return QByteArray(picture->data().data(), picture->data().size());
        }
    }
    return QByteArray();
}

QByteArray
EmbeddedImageData::Private::getFrontCoverFromMp4(TagLib::MP4::Tag* mp4Tags) const
{
    TagLib::MP4::Item coverArtItem = mp4Tags->item("covr");
    if (coverArtItem.isValid())
    {
        TagLib::MP4::CoverArtList coverArtList = coverArtItem.toCoverArtList();
        TagLib::MP4::CoverArt& frontCover = coverArtList.front();
        return QByteArray(frontCover.data().data(), frontCover.data().size());
    }
    return QByteArray();
}

QByteArray
EmbeddedImageData::Private::getFrontCoverFromApe(TagLib::APE::Tag* apeTags) const
{
    TagLib::APE::ItemListMap lstApe = apeTags->itemListMap();
    TagLib::APE::ItemListMap::ConstIterator itApe;

    /* The cover art tag for APEv2 tags starts with the filename as string
     * with zero termination followed by the actual picture data */
    itApe = lstApe.find("COVER ART (FRONT)");
    if (itApe != lstApe.end()) {
        TagLib::ByteVector pictureData = (*itApe).second.binaryData();
        int position = pictureData.find('\0');
        if (position >= 0) {
            position += 1;
            return QByteArray(pictureData.data() + position, pictureData.size() - position);
        }
    }
    return QByteArray();
}
QByteArray
EmbeddedImageData::Private::getFrontCoverFromAsf(TagLib::ASF::Tag* asfTags) const
{
    TagLib::ASF::AttributeList lstASF = asfTags->attribute("WM/Picture");
    for (const auto& attribute : qAsConst(lstASF)) {
        TagLib::ASF::Picture picture = attribute.toPicture();
        if (picture.type() == TagLib::ASF::Picture::FrontCover) {
            TagLib::ByteVector pictureData = picture.picture();
            return QByteArray(pictureData.data(), pictureData.size());
        }
    }
    return QByteArray();
}
