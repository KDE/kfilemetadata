#include "taglibwriter.h"

#include <taglib.h>
#include <tstring.h>
#include <id3v2tag.h>
#include <fileref.h>

using namespace KFileMetaData;

TagLibWriter::TagLibWriter(QObject* parent)
    : WriterPlugin(parent)
{
}

TagLib::String q2t(const QString& q)
{
    return TagLib::String(q.toStdWString());
}

QStringList TagLibWriter::writeMimetypes() const
{
    QStringList types = {
        QStringLiteral("audio/flac"),
        QStringLiteral("audio/mp4"),
        QStringLiteral("audio/mpeg"),
        QStringLiteral("audio/mpeg3"),
        QStringLiteral("audio/ogg"),
        QStringLiteral("audio/opus"),
        QStringLiteral("audio/x-mpeg"),
        QStringLiteral("audio/x-musepack"),
        QStringLiteral("audio/x-opus+ogg"),
        QStringLiteral("audio/x-vorbis+ogg"),
    };

    return types;
}

void TagLibWriter::write(const WriteData& data)
{
    const QString fileUrl = data.inputUrl();
    const PropertyMap properties = data.getAllProperties();

    TagLib::FileRef file(fileUrl.toUtf8().constData(), true);
    if (file.isNull()) {
        return;
    }

    TagLib::Tag* tags = file.tag();

    TagLib::String title;
    TagLib::String artist;
    TagLib::String album;
    TagLib::String genre;
    TagLib::String comment;

    if (properties.contains(Property::Title)) {
        title = q2t(properties.value(Property::Title).toString());
        tags->setTitle(title);
    }

    if (properties.contains(Property::Artist)) {
        artist = q2t(properties.value(Property::Artist).toString());
        tags->setArtist(artist);
    }

    if (properties.contains(Property::Album)) {
        album = q2t(properties.value(Property::Album).toString());
        tags->setAlbum(album);
    }

    if (properties.contains(Property::TrackNumber)) {
        int trackNumber = properties.value(Property::TrackNumber).toInt();
        //taglib requires uint
        if (trackNumber >= 0) {
            tags->setTrack(trackNumber);
        }
    }

    if (properties.contains(Property::ReleaseYear)) {
        int year = properties.value(Property::ReleaseYear).toInt();
        //taglib requires uint
        if (year >= 0) {
            tags->setYear(year);
        }
    }

    if (properties.contains(Property::Genre)) {
        genre = q2t(properties.value(Property::Genre).toString());
        tags->setGenre(genre);
    }

    if (properties.contains(Property::Comment)) {
        comment = q2t(properties.value(Property::Comment).toString());
        tags->setComment(comment);
    }


    file.save();
}
