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
#include <QDateTime>

using namespace KFileMetaData;

FFmpegExtractor::FFmpegExtractor(QObject* parent, const QVariantList&)
: ExtractorPlugin(parent)
{
}

QStringList FFmpegExtractor::mimetypes() const
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

void FFmpegExtractor::extract(ExtractionResult* result)
{
    AVFormatContext* fmt_ctx = NULL;

    av_register_all();

    QByteArray arr = result->inputUrl().toUtf8();

    fmt_ctx = avformat_alloc_context();
    if (int ret = avformat_open_input(&fmt_ctx, arr.data(), NULL, NULL)) {
        kError() << "avformat_open_input error: " << ret;
        return;
    }

    int ret = avformat_find_stream_info(fmt_ctx, NULL);
    if (ret < 0) {
        kError() << "avform_find_stream_info error: " << ret;
        return;
    }

    result->addType(Type::Video);

    int totalSecs = fmt_ctx->duration / AV_TIME_BASE;
    int bitrate = fmt_ctx->bit_rate;

    result->add(Property::Duration, totalSecs);
    result->add(Property::BitRate, bitrate);

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

                result->add(Property::Width, codec->width);
                result->add(Property::Height, codec->height);
                if (aspectRatio)
                    result->add(Property::AspectRatio, aspectRatio);
                if (frameRate)
                    result->add(Property::FrameRate, frameRate);
            }
        }
    }

    AVDictionary* dict = fmt_ctx->metadata;
    AVDictionaryEntry* entry;

    entry = av_dict_get(dict, "title", NULL, 0);
    if (entry) {
        result->add(Property::Title, QString::fromUtf8(entry->value));
    }


    entry = av_dict_get(dict, "author", NULL, 0);
    if (entry) {
        result->add(Property::Author, QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "copyright", NULL, 0);
    if (entry) {
        result->add(Property::Copyright, QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "comment", NULL, 0);
    if (entry) {
        result->add(Property::Comment, QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "album", NULL, 0);
    if (entry) {
        result->add(Property::Album, QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "genre", NULL, 0);
    if (entry) {
        result->add(Property::Genre, QString::fromUtf8(entry->value));
    }

    entry = av_dict_get(dict, "track", NULL, 0);
    if (entry) {
        QString value = QString::fromUtf8(entry->value);

        bool ok = false;
        int track = value.toInt(&ok);
        if (ok && track)
            result->add(Property::TrackNumber, track);
    }

    entry = av_dict_get(dict, "year", NULL, 0);
    if (entry) {
        int year = QString::fromUtf8(entry->value).toInt();
        result->add(Property::ReleaseYear, year);
    }

    avformat_close_input(&fmt_ctx);
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::FFmpegExtractor, "kfilemetadata_ffmpegextractor")
