/*
    This file is part of the KFileMetaData project that extract metadata from files
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>
    Copyright 2017 Matthieu Gallien <matthieu_gallien@yahoo.fr>

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


#include "qtmultimediaextractor.h"

#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QMutexLocker>

using namespace KFileMetaData;

namespace KFileMetaData
{

class ExecutionHelper: public QObject
{

    Q_OBJECT

public:

    ExecutionHelper(QSemaphore &synchronizeSemaphore, QMutex &synchronizeMutex, QObject *parent = nullptr)
        : QObject(parent), mSynchronizeSemaphore(synchronizeSemaphore), mSynchronizeMutex(synchronizeMutex)
    {
    }

    virtual ~ExecutionHelper()
    {
        delete mMetadataExtractor;
    }

public Q_SLOTS:

    void setMediaSource(const QUrl &mediaSource)
    {
        album = {};
        title = {};
        artist = {};
        albumArtist = {};
        genre = {};
        comment = {};
        composer = {};
        trackNumber = {};
        releaseYear = {};
        channels = {};
        duration = {};

        mMetadataExtractor->setMedia(mediaSource);
    }

    void init()
    {
        mMetadataExtractor = new QMediaPlayer;
        connect(mMetadataExtractor, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(metaDataAvailableChanged(bool)), Qt::DirectConnection);
        connect(mMetadataExtractor, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(playerError()));
    }

    void metaDataAvailableChanged(bool available)
    {
        if (available && mMetadataExtractor->isMetaDataAvailable()) {
            QMutexLocker locker(&mSynchronizeMutex);

            album = mMetadataExtractor->metaData(QMediaMetaData::AlbumTitle);
            title = mMetadataExtractor->metaData(QMediaMetaData::Title);
            artist = mMetadataExtractor->metaData(QMediaMetaData::ContributingArtist).toStringList().join(QStringLiteral(", "));
            albumArtist = mMetadataExtractor->metaData(QMediaMetaData::AlbumArtist);
            genre = mMetadataExtractor->metaData(QMediaMetaData::Genre);
            comment = mMetadataExtractor->metaData(QMediaMetaData::Comment);
            composer = mMetadataExtractor->metaData(QMediaMetaData::Composer);
            trackNumber = mMetadataExtractor->metaData(QMediaMetaData::TrackNumber);
            releaseYear = mMetadataExtractor->metaData(QMediaMetaData::Year);
            channels = mMetadataExtractor->metaData(QMediaMetaData::ChannelCount);
            duration = mMetadataExtractor->metaData(QMediaMetaData::Duration).toLongLong() / 1000.0;

            mSynchronizeSemaphore.release(1);
        }
    }

    void playerError()
    {
        album = {};
        title = {};
        artist = {};
        albumArtist = {};
        genre = {};
        comment = {};
        composer = {};
        trackNumber = {};
        releaseYear = {};
        channels = {};
        duration = {};
        mSynchronizeSemaphore.release(1);
    }

private:

    QSemaphore &mSynchronizeSemaphore;

    QMutex &mSynchronizeMutex;

public:

    QMediaPlayer *mMetadataExtractor = nullptr;

    QVariant album;

    QVariant title;

    QVariant artist;

    QVariant albumArtist;

    QVariant genre;

    QVariant comment;

    QVariant composer;

    QVariant trackNumber;

    QVariant releaseYear;

    QVariant channels;

    QVariant duration;

};

}

QtMultimediaExtractor::QtMultimediaExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{
    mHelper = new ExecutionHelper(mSynchronizeSemaphore, mSynchronizeMutex);

    mHelper->moveToThread(&mRunnerThread);

    mRunnerThread.start();

    QMetaObject::invokeMethod(mHelper, "init", Qt::BlockingQueuedConnection);
}

QtMultimediaExtractor::~QtMultimediaExtractor()
{
    mRunnerThread.quit();
    mRunnerThread.wait();

    delete mHelper;
}

QStringList QtMultimediaExtractor::mimetypes() const
{
    QStringList types;
    // MP3, FLAC, Vorbis, Opus,  MPC, Speex, WavPack TrueAudio, WAV, AIFF, MP4 and ASF files.
    // MP3
    types << QStringLiteral("audio/mpeg");
    types << QStringLiteral("audio/mpeg3"); types << QStringLiteral("audio/x-mpeg");

    // M4A
    types << QStringLiteral("audio/mp4");

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

void QtMultimediaExtractor::extract(ExtractionResult* result)
{
    const QString fileUrl = result->inputUrl();

    QMetaObject::invokeMethod(mHelper, "setMediaSource", Qt::BlockingQueuedConnection, Q_ARG(QUrl, QUrl::fromLocalFile(fileUrl)));

    mSynchronizeSemaphore.acquire(1);
    QMutexLocker locker(&mSynchronizeMutex);

    if (mHelper->mMetadataExtractor->isMetaDataAvailable()) {
        result->addType(Type::Audio);

        result->add(Property::Album, mHelper->album);
        result->add(Property::Title, mHelper->title);
        result->add(Property::Artist, mHelper->artist);
        result->add(Property::AlbumArtist, mHelper->albumArtist);
        result->add(Property::Genre, mHelper->genre);
        result->add(Property::Comment, mHelper->comment);
        result->add(Property::Composer, mHelper->composer);
        result->add(Property::TrackNumber, mHelper->trackNumber);
        result->add(Property::ReleaseYear, mHelper->releaseYear);
        result->add(Property::Channels, mHelper->channels);
        result->add(Property::Duration, mHelper->duration);
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

#include "qtmultimediaextractor.moc"
