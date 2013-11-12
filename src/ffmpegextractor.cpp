/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>

    Code adapted from Strigi FFmpeg Analyzer -
    Copyright (C) 2010 Evgeny Egorochkin <phreedom.stdin@gmail.com>
    Copyright (C) 2011 Tirtha Chatterjee <tirtha.p.chatterjee@gmail.com>

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


#include "ffmpegextractor.h"

#ifdef __cplusplus
#define __STDC_CONSTANT_MACROS
#ifdef _STDINT_H
#undef _STDINT_H
#endif
# include <stdint.h>
#endif

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
#include <libavcodec/avcodec.h>
}

#include <KDebug>
#include <QtCore/QDateTime>

using namespace KMetaData;

FFmpegExtractor::FFmpegExtractor(QObject* parent, const QVariantList&)
: ExtractorPlugin(parent)
{
}

QStringList FFmpegExtractor::mimetypes()
{
    QStringList types;

    types << QLatin1String("video/x-ms-asf");
    types << QLatin1String("video/x-msvideo");
    types << QLatin1String("video/x-flv");
    types << QLatin1String("video/quicktime");
    types << QLatin1String("video/mpeg");
    types << QLatin1String("video/x-ms-wmv");
    types << QLatin1String("video/mp4");
    types << QLatin1String("video/x-matroska");
    types << QLatin1String("video/webm");

    return types;
}

QVariantMap FFmpegExtractor::extract(const QString& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    AVFormatContext* fmt_ctx = NULL;

    av_register_all();

    QByteArray arr = fileUrl.toUtf8();

    fmt_ctx = avformat_alloc_context();
    if (int ret = avformat_open_input(&fmt_ctx, arr.data(), NULL, NULL)) {
        kError() << "avformat_open_input error: " << ret;
        return QVariantMap();
    }

    int ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        kError() << "avform_find_stream_info error: " << ret;
        return QVariantMap();
    }

    QVariantMap metadata;
    // TODO: Add types!!

    /*if (fmt_ctx->nb_streams == 1 && fmt_ctx->streams[0]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
        fileRes.addType(NMM::MusicPiece());
    } else {
        fileRes.addType(NFO::Video());
    }*/

    int totalSecs = fmt_ctx->duration / AV_TIME_BASE;
    int bitrate = fmt_ctx->bit_rate;

    metadata.insert("duration", totalSecs);
    metadata.insert("averageBitrate", bitrate);

    for (uint i = 0; i < fmt_ctx->nb_streams; i++) {
        const AVStream* stream = fmt_ctx->streams[i];
        const AVCodecContext* codec = stream->codec;

        if (codec->codec_type == AVMEDIA_TYPE_AUDIO || codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            /*
            if( codec->codec_type == AVMEDIA_TYPE_AUDIO ) {
                subRes.addType( NFO::Audio() );
                subRes.addProperty( NFO::sampleRate(), codec->sample_rate );
                subRes.addProperty( NFO::channels(), codec->channels );

                //TODO: Fetch Sample Format
            }*/

            if (codec->codec_type == AVMEDIA_TYPE_VIDEO) {
                int aspectRatio = codec->sample_aspect_ratio.num;
                int frameRate = stream->avg_frame_rate.num;

                if (codec->sample_aspect_ratio.den)
                    aspectRatio /= codec->sample_aspect_ratio.den;
                if (stream->avg_frame_rate.den)
                    frameRate /= stream->avg_frame_rate.den;

                metadata.insert("width", codec->width);
                metadata.insert("height", codec->height);
                metadata.insert("aspectRatio", aspectRatio);
                metadata.insert("frameRate", frameRate);
            }
        }
    }

    AVDictionary* dict = fmt_ctx->metadata;
    AVDictionaryEntry* entry;

    entry = av_dict_get(dict, "title", NULL, 0);
    if (entry) {
        metadata.insert("title", QString::fromUtf8(entry->value));
    }


    entry = av_dict_get(dict, "author", NULL, 0);
    if (entry) {
        metadata.insert("author", QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "copyright", NULL, 0);
    if (entry) {
        metadata.insert("copyright", QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "comment", NULL, 0);
    if (entry) {
        metadata.insert("comment", QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "album", NULL, 0);
    if (entry) {
        metadata.insert("album", QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "genre", NULL, 0);
    if (entry) {
        metadata.insert("genre", QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "track", NULL, 0);
    if (entry) {
        QString value = QString::fromUtf8(entry->value);

        bool ok = false;
        int track = value.toInt(&ok);
        if (ok && track)
            metadata.insert("track", track);
    }

    entry = av_dict_get(dict, "year", NULL, 0);
    if (entry) {
        //FIXME: Parse date in different formats
        QString value = QString::fromUtf8(entry->value);
        QDate date = QDate::fromString("yyyy", value);
        metadata.insert("contentCreated", date);
    }

    avformat_close_input(&fmt_ctx);

    return metadata;
}

KMETADATA_EXPORT_EXTRACTOR(KMetaData::FFmpegExtractor, "nepomukffmpegextractor")
