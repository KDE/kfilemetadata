/*
    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "embeddedimagedata.h"
#include "kfilemetadata_debug.h"
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

#include <QByteArray>
#include <QMimeDatabase>

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
    void writeFrontCover(const QString &fileUrl, const QString &mimeType, const QByteArray &pictureData);
    void writeFrontCoverToID3(TagLib::ID3v2::Tag* Id3Tags, const QByteArray &pictureData);
    void writeFrontCoverToFlacPicture(TagLib::List<TagLib::FLAC::Picture *> lstPic, const QByteArray &pictureData);
    void writeFrontCoverToMp4(TagLib::MP4::Tag* mp4Tags, const QByteArray &pictureData);
    void writeFrontCoverToApe(TagLib::APE::Tag* apeTags, const QByteArray &pictureData);
    void writeFrontCoverToAsf(TagLib::ASF::Tag* asfTags, const QByteArray &pictureData);
    TagLib::String determineMimeType(const QByteArray &pictureData);
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

    const auto fileMimeType = d->mMimeDatabase.mimeTypeForFile(fileUrl);
    if (fileMimeType.name().startsWith(QLatin1String("audio/"))) {
        if (types & EmbeddedImageData::FrontCover) {
            imageData.insert(EmbeddedImageData::FrontCover, d->getFrontCover(fileUrl, fileMimeType.name()));
        }
    }

    return imageData;
}

void
EmbeddedImageData::writeImageData(const QString &fileUrl,
                             QMap<EmbeddedImageData::ImageType, QByteArray> &imageData)
{
    const auto fileMimeType = d->mMimeDatabase.mimeTypeForFile(fileUrl);
    QMap<EmbeddedImageData::ImageType, QByteArray>::iterator frontCover = imageData.find(EmbeddedImageData::FrontCover);
    if (fileMimeType.name().startsWith(QLatin1String("audio/"))) {
        if (frontCover != imageData.end()) {
            d->writeFrontCover(fileUrl, fileMimeType.name(), frontCover.value());
        }
    }
}

QByteArray
EmbeddedImageData::Private::getFrontCover(const QString &fileUrl,
                                          const QString &mimeType) const
{
    TagLib::FileStream stream(fileUrl.toUtf8().constData(), true);
    if (!stream.isOpen()) {
        qCWarning(KFILEMETADATA_LOG) << "Unable to open file readonly: " << fileUrl;
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

void
EmbeddedImageData::Private::writeFrontCover(const QString &fileUrl,
                                          const QString &mimeType, const QByteArray &pictureData)
{
    TagLib::FileStream stream(fileUrl.toUtf8().constData(), false);
    if (!stream.isOpen()) {
        qWarning() << "Unable to open file: " << fileUrl;
        return;
    }

    if ((mimeType == QLatin1String("audio/mpeg"))
            || (mimeType == QLatin1String("audio/mpeg3"))
            || (mimeType == QLatin1String("audio/x-mpeg"))) {

        // Handling multiple tags in mpeg files.
        TagLib::MPEG::File mpegFile(&stream, TagLib::ID3v2::FrameFactory::instance(), false);
        if (mpegFile.ID3v2Tag()) {
            writeFrontCoverToID3(mpegFile.ID3v2Tag(), pictureData);
        }
        mpegFile.save();
    } else if (mimeType == QLatin1String("audio/x-aiff")) {

        TagLib::RIFF::AIFF::File aiffFile(&stream, false);
        if (aiffFile.hasID3v2Tag()) {
            writeFrontCoverToID3(aiffFile.tag(), pictureData);
        }
        aiffFile.save();
    } else if ((mimeType == QLatin1String("audio/wav"))
               || (mimeType == QLatin1String("audio/x-wav"))) {

        TagLib::RIFF::WAV::File wavFile(&stream, false);
        if (wavFile.hasID3v2Tag()) {
            writeFrontCoverToID3(wavFile.ID3v2Tag(), pictureData);
        }
        wavFile.save();
    } else if (mimeType == QLatin1String("audio/mp4")) {

        TagLib::MP4::File mp4File(&stream, false);
        if (mp4File.tag()) {
            writeFrontCoverToMp4(mp4File.tag(), pictureData);
        }
        mp4File.save();
    } else if (mimeType == QLatin1String("audio/x-musepack")) {

        TagLib::MPC::File mpcFile(&stream, false);
        if (mpcFile.APETag()) {
           writeFrontCoverToApe(mpcFile.APETag(), pictureData);
        }
        mpcFile.save();
    } else if (mimeType == QLatin1String("audio/x-ape")) {

        TagLib::APE::File apeFile(&stream, false);
        if (apeFile.hasAPETag()) {
            writeFrontCoverToApe(apeFile.APETag(), pictureData);
        }
        apeFile.save();
    } else if (mimeType == QLatin1String("audio/x-wavpack")) {

        TagLib::WavPack::File wavpackFile(&stream, false);
        if (wavpackFile.hasAPETag()) {
            writeFrontCoverToApe(wavpackFile.APETag(), pictureData);
        }
        wavpackFile.save();
    } else if (mimeType == QLatin1String("audio/x-ms-wma")) {

        TagLib::ASF::File asfFile(&stream, false);
        TagLib::ASF::Tag* asfTags = dynamic_cast<TagLib::ASF::Tag*>(asfFile.tag());
        if (asfTags) {
            writeFrontCoverToAsf(asfTags, pictureData);
        }
        asfFile.save();
    } else if (mimeType == QLatin1String("audio/flac")) {

        TagLib::FLAC::File flacFile(&stream, TagLib::ID3v2::FrameFactory::instance(), false);
        writeFrontCoverToFlacPicture(flacFile.pictureList(), pictureData);
        flacFile.save();
    } else if ((mimeType == QLatin1String("audio/ogg"))
               || (mimeType == QLatin1String("audio/x-vorbis+ogg"))) {

        TagLib::Ogg::Vorbis::File oggFile(&stream, false);
        if (oggFile.tag()) {
            writeFrontCoverToFlacPicture(oggFile.tag()->pictureList(), pictureData);
        }
        oggFile.save();
    }
    else if ((mimeType == QLatin1String("audio/opus"))
             || (mimeType == QLatin1String("audio/x-opus+ogg"))) {

        TagLib::Ogg::Opus::File opusFile(&stream, false);
        if (opusFile.tag()) {
            writeFrontCoverToFlacPicture(opusFile.tag()->pictureList(), pictureData);
        }
        opusFile.save();
    } else if (mimeType == QLatin1String("audio/speex") || mimeType == QLatin1String("audio/x-speex")) {

        TagLib::Ogg::Speex::File speexFile(&stream, false);
        if (speexFile.tag()) {
            writeFrontCoverToFlacPicture(speexFile.tag()->pictureList(), pictureData);
        }
        speexFile.save();
    }
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

void
EmbeddedImageData::Private::writeFrontCoverToID3(TagLib::ID3v2::Tag* Id3Tags, const QByteArray &pictureData)
{
    // Try to update existing front cover frame first
    TagLib::ID3v2::FrameList lstID3v2;
    lstID3v2 = Id3Tags->frameListMap()["APIC"];
    for (auto& frame : qAsConst(lstID3v2))
    {
        auto *frontCoverFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
        if (frontCoverFrame->type() == frontCoverFrame->FrontCover) {
            frontCoverFrame->setPicture(TagLib::ByteVector(static_cast<const char *>(pictureData.data()), pictureData.size()));
            frontCoverFrame->setMimeType(determineMimeType(pictureData));
            return;
        }
    }
    auto pictureFrame = new TagLib::ID3v2::AttachedPictureFrame;
    pictureFrame->setPicture(TagLib::ByteVector(pictureData.data(), pictureData.size()));
    pictureFrame->setType(pictureFrame->FrontCover);
    pictureFrame->setMimeType(determineMimeType(pictureData));
    Id3Tags->addFrame(pictureFrame);
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

void
EmbeddedImageData::Private::writeFrontCoverToFlacPicture(TagLib::List<TagLib::FLAC::Picture *> lstPic, const QByteArray &pictureData)
{
    for (const auto &picture : qAsConst(lstPic)) {
        if (picture->type() == picture->FrontCover) {
            picture->setData(TagLib::ByteVector(pictureData.data(), pictureData.size()));
            picture->setMimeType(determineMimeType(pictureData));
            return ;
        }
    }
    auto flacPicture = new TagLib::FLAC::Picture;
    flacPicture->setMimeType(determineMimeType(pictureData));
    flacPicture->setData(TagLib::ByteVector(pictureData.data(), pictureData.size()));
    lstPic.append(flacPicture);
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

void
EmbeddedImageData::Private::writeFrontCoverToMp4(TagLib::MP4::Tag* mp4Tags, const QByteArray &pictureData)
{
    TagLib::MP4::Item coverArtItem = mp4Tags->item("covr");
    TagLib::MP4::CoverArtList coverArtList;
    TagLib::MP4::CoverArt coverArt(TagLib::MP4::CoverArt::Format::Unknown, TagLib::ByteVector(pictureData.data(), pictureData.size()));
    if (coverArtItem.isValid())
    {
        coverArtList = coverArtItem.toCoverArtList();
        coverArtList.clear();
    }
    coverArtList.append(coverArt);
    mp4Tags->setItem("covr", coverArtList);
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

void
EmbeddedImageData::Private::writeFrontCoverToApe(TagLib::APE::Tag* apeTags, const QByteArray &pictureData)
{
    /* The cover art tag for APEv2 tags starts with the filename as string
     * with zero termination followed by the actual picture data */
    TagLib::ByteVector imageData;
    TagLib::String name;
    if (determineMimeType(pictureData) == TagLib::String("image/png")) {
        name = "frontCover.png";
    } else {
        name = "frontCover.jpeg";
    }
    imageData.append(name.data(TagLib::String::UTF8));
    imageData.append('\0');
    imageData.append(TagLib::ByteVector(pictureData.constData(), pictureData.size()));
    apeTags->setData("COVER ART (FRONT)", imageData);
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

void
EmbeddedImageData::Private::writeFrontCoverToAsf(TagLib::ASF::Tag* asfTags, const QByteArray &pictureData)
{
    TagLib::ASF::AttributeList lstASF = asfTags->attribute("WM/Picture");
    for (const auto& attribute : qAsConst(lstASF)) {
        TagLib::ASF::Picture picture = attribute.toPicture();
        if (picture.type() == TagLib::ASF::Picture::FrontCover) {
            picture.setPicture(TagLib::ByteVector(pictureData.constData(), pictureData.size()));
            picture.setMimeType(determineMimeType(pictureData));
            TagLib::ByteVector pictureData = picture.picture();
            return;
        }
    }
    TagLib::ASF::Picture picture;
    picture.setPicture(TagLib::ByteVector(pictureData.constData(), pictureData.size()));
    picture.setType(TagLib::ASF::Picture::FrontCover);
    lstASF.append(picture);
}


TagLib::String EmbeddedImageData::Private::determineMimeType(const QByteArray &pictureData)
{
    if (pictureData.startsWith(QByteArray::fromHex("89504E470D0A1A0A"))) {
        return TagLib::String("image/png");
    } else if (pictureData.startsWith(QByteArray::fromHex("FFD8FFDB")) ||
               pictureData.startsWith(QByteArray::fromHex("FFD8FFE000104A4649460001")) ||
               pictureData.startsWith(QByteArray::fromHex("FFD8FFEE")) ||
               pictureData.startsWith(QByteArray::fromHex("FFD8FFE1"))) {
        return TagLib::String("image/jpeg");
    } else {
        return TagLib::String();
    }
}

