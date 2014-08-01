/*
 * This file is part of the KFileMetaData project
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "propertyinfo.h"

#include <KLocalizedString>

// FIXME: vhanda
//static const KCatalogLoader loader("kfilemetadata");

using namespace KFileMetaData;

class PropertyInfo::Private {
public:
    Property::Property prop;
    QString name;
    QString displayName;
    QVariant::Type valueType;
    bool shouldBeIndexed;
};

PropertyInfo::PropertyInfo(Property::Property property)
    : d(new Private)
{
    d->prop = property;
    d->shouldBeIndexed = true;

    switch (property) {
        case Property::Album:
            d->name = QStringLiteral("album");
            d->displayName = i18nc("@label music album", "Album");
            d->valueType = QVariant::String;
            break;

        case Property::AlbumArtist:
            d->name = QStringLiteral("albumArtist");
            d->displayName = i18nc("@label", "Album Artist");
            d->valueType = QVariant::StringList;
            break;

        case Property::Artist:
            d->name = QStringLiteral("artist");
            d->displayName = i18nc("@label", "Artist");
            d->valueType = QVariant::StringList;
            break;

        case Property::AspectRatio:
            d->name = QStringLiteral("aspectRatio");
            d->displayName = i18nc("@label", "Aspect Ratio");
            d->valueType = QVariant::Int;
            break;

        case Property::Author:
            d->name = QStringLiteral("author");
            d->displayName = i18nc("@label", "Author");
            d->valueType = QVariant::StringList;
            break;

        case Property::BitRate:
            d->name = QStringLiteral("bitRate");
            d->displayName = i18nc("@label", "Bitrate");
            d->valueType = QVariant::Int;
            break;

        case Property::Channels:
            d->name = QStringLiteral("channels");
            d->displayName = i18nc("@label", "Channels");
            d->valueType = QVariant::Int;
            break;

        case Property::Comment:
            d->name = QStringLiteral("comment");
            d->displayName = i18nc("@label", "Comment");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Composer:
            d->name = QStringLiteral("composer");
            d->displayName = i18nc("@label", "Composer");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Copyright:
            d->name = QStringLiteral("copyright");
            d->displayName = i18nc("@label", "Copyright");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::CreationDate:
            d->name = QStringLiteral("creationDate");
            d->displayName = i18nc("@label", "Creation Date");
            d->valueType = QVariant::String;
            break;

        case Property::Duration:
            d->name = QStringLiteral("duration");
            d->displayName = i18nc("@label", "Duration");
            d->valueType = QVariant::Int;
            break;

        case Property::Empty:
            d->name = QStringLiteral("empty");
            d->valueType = QVariant::Invalid;
            break;

        case Property::FrameRate:
            d->name = QStringLiteral("frameRate");
            d->displayName = i18nc("@label", "Frame Rate");
            d->valueType = QVariant::Int;
            break;

        case Property::Generator:
            d->name = QStringLiteral("generator");
            d->displayName = i18nc("@label", "Software used to Generate the document");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Genre:
            d->name = QStringLiteral("genre");
            d->displayName = i18nc("@label music genre", "Genre");
            d->valueType = QVariant::StringList;
            d->shouldBeIndexed = false;
            break;

        case Property::Height:
            d->name = QStringLiteral("height");
            d->displayName = i18nc("@label", "Height");
            d->valueType = QVariant::Int;
            break;

        case Property::ImageDateTime:
            d->name = QStringLiteral("imageDateTime");
            d->displayName = i18nc("@label EXIF", "Image Date Time");
            d->valueType = QVariant::DateTime;
            break;

        case Property::ImageMake:
            d->name = QStringLiteral("imageMake");
            d->displayName = i18nc("@label EXIF", "Image Make");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::ImageModel:
            d->name = QStringLiteral("imageModel");
            d->displayName = i18nc("@label EXIF", "Image Model");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::ImageOrientation:
            d->name = QStringLiteral("imageOrientation");
            d->displayName = i18nc("@label EXIF", "Image Orientation");
            d->valueType = QVariant::Int;
            break;

        case Property::Keywords:
            d->name = QStringLiteral("keywords");
            d->displayName = i18nc("@label", "Keywords");
            d->valueType = QVariant::StringList;
            d->shouldBeIndexed = false;
            break;

        case Property::Langauge:
            d->name = QStringLiteral("language");
            d->displayName = i18nc("@label", "Language");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::LineCount:
            d->name = QStringLiteral("lineCount");
            d->displayName = i18nc("@label number of lines", "Line Count");
            d->valueType = QVariant::Int;
            break;

        case Property::Lyricist:
            d->name = QStringLiteral("lyricist");
            d->displayName = i18nc("@label", "Lyricist");
            d->valueType = QVariant::StringList;
            d->shouldBeIndexed = false;
            break;

        case Property::PageCount:
            d->name = QStringLiteral("pageCount");
            d->displayName = i18nc("@label", "Page Count");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoApertureValue:
            d->name = QStringLiteral("photoApertureValue");
            d->displayName = i18nc("@label EXIF", "Photo Aperture Value");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoDateTimeOriginal:
            d->name = QStringLiteral("photoDateTimeOriginal");
            d->displayName = i18nc("@label EXIF", "Photo Original Date Time");
            d->valueType = QVariant::DateTime;
            break;

        case Property::PhotoExposureBiasValue:
            d->name = QStringLiteral("photoExposureBiasValue");
            d->displayName = i18nc("@label EXIF", "Photo Exposure Bias");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoExposureTime:
            d->name = QStringLiteral("photoExposureTime");
            d->displayName = i18nc("@label EXIF", "Photo Exposure Time");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoFlash:
            d->name = QStringLiteral("photoFlash");
            d->displayName = i18nc("@label EXIF", "Photo Flash");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoFNumber:
            d->name = QStringLiteral("photoFNumber");
            d->displayName = i18nc("@label EXIF", "Photo F Number");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoFocalLength:
            d->name = QStringLiteral("photoFocalLength");
            d->displayName = i18nc("@label EXIF", "Photo Focal Length");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoFocalLengthIn35mmFilm:
            d->name = QStringLiteral("photoFocalLengthIn35mmFilm");
            d->displayName = i18nc("@label EXIF", "Photo Focal Length 35mm");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoISOSpeedRatings:
            d->name = QStringLiteral("photoISOSpeedRatings");
            d->displayName = i18nc("@label EXIF", "Photo ISO Speed Rating");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoMeteringMode:
            d->name = QStringLiteral("photoMeteringMode");
            d->displayName = i18nc("@label EXIF", "Photo Metering Mode");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoPixelXDimension:
            d->name = QStringLiteral("photoPixelXDimension");
            d->displayName = i18nc("@label EXIF", "Photo X Dimension");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoPixelYDimension:
            d->name = QStringLiteral("photoPixelYDimension");
            d->displayName = i18nc("@label EXIF", "Photo Y Dimension");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoSaturation:
            d->name = QStringLiteral("photoSaturation");
            d->displayName = i18nc("@label EXIF", "Photo Saturation");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoSharpness:
            d->name = QStringLiteral("photoSharpness");
            d->displayName = i18nc("@label EXIF", "Photo Sharpness");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoWhiteBalance:
            d->name = QStringLiteral("photoWhiteBalance");
            d->displayName = i18nc("@label EXIF", "Photo White Balance");
            d->valueType = QVariant::Int;
            break;

        case Property::Publisher:
            d->name = QStringLiteral("publisher");
            d->displayName = i18nc("@label", "Publisher");
            d->valueType = QVariant::String;
            break;

        case Property::ReleaseYear:
            d->name = QStringLiteral("releaseYear");
            d->displayName = i18nc("@label", "Release Year");
            d->valueType = QVariant::Int;
            break;

        case Property::SampleRate:
            d->name = QStringLiteral("sampleRate");
            d->displayName = i18nc("@label", "Sample Rate");
            d->valueType = QVariant::Int;
            break;

        case Property::Subject:
            d->name = QStringLiteral("subject");
            d->displayName = i18nc("@label", "Subject");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Title:
            d->name = QStringLiteral("title");
            d->displayName = i18nc("@label", "Title");
            d->valueType = QVariant::String;
            break;

        case Property::TrackNumber:
            d->name = QStringLiteral("trackNumber");
            d->displayName = i18nc("@label music track number", "Track Number");
            d->valueType = QVariant::Int;
            break;

        case Property::Width:
            d->name = QStringLiteral("width");
            d->displayName = i18nc("@label", "Width");
            d->valueType = QVariant::Int;
            break;

        case Property::WordCount:
            d->name = QStringLiteral("wordCount");
            d->displayName = i18nc("@label number of words", "Word Count");
            d->valueType = QVariant::Int;
            break;

        default:
            break;
    }

    if (d->valueType == QVariant::Int || d->valueType == QVariant::DateTime ||
        d->valueType == QVariant::Double)
    {
        d->shouldBeIndexed = false;
    }
}

PropertyInfo::PropertyInfo(const PropertyInfo& pi)
    : d(new Private(*pi.d))
{
}

PropertyInfo::~PropertyInfo()
{
    delete d;
}

QString PropertyInfo::displayName() const
{
    return d->displayName;
}

QString PropertyInfo::name() const
{
    return d->name;
}

Property::Property PropertyInfo::property() const
{
    return d->prop;
}

QVariant::Type PropertyInfo::valueType() const
{
    return d->valueType;
}

bool PropertyInfo::shouldBeIndexed() const
{
    return d->shouldBeIndexed;
}

PropertyInfo PropertyInfo::fromName(const QString& name)
{
    static QHash<QString, Property::Property> propertyHash;

    // FIXME: Multi-threading?
    if (propertyHash.isEmpty()) {
        propertyHash.insert(QStringLiteral("bitrate"), Property::BitRate);
        propertyHash.insert(QStringLiteral("channels"), Property::Channels);
        propertyHash.insert(QStringLiteral("duration"), Property::Duration);
        propertyHash.insert(QStringLiteral("genre"), Property::Genre);
        propertyHash.insert(QStringLiteral("samplerate"), Property::SampleRate);
        propertyHash.insert(QStringLiteral("tracknumber"), Property::TrackNumber);
        propertyHash.insert(QStringLiteral("releaseyear"), Property::ReleaseYear);
        propertyHash.insert(QStringLiteral("comment"), Property::Comment);
        propertyHash.insert(QStringLiteral("artist"), Property::Artist);
        propertyHash.insert(QStringLiteral("album"), Property::Album);
        propertyHash.insert(QStringLiteral("albumartist"), Property::AlbumArtist);
        propertyHash.insert(QStringLiteral("composer"), Property::Composer);
        propertyHash.insert(QStringLiteral("lyricist"), Property::Lyricist);
        propertyHash.insert(QStringLiteral("author"), Property::Author);
        propertyHash.insert(QStringLiteral("title"), Property::Title);
        propertyHash.insert(QStringLiteral("subject"), Property::Subject);
        propertyHash.insert(QStringLiteral("generator"), Property::Generator);
        propertyHash.insert(QStringLiteral("pagecount"), Property::PageCount);
        propertyHash.insert(QStringLiteral("wordcount"), Property::WordCount);
        propertyHash.insert(QStringLiteral("linecount"), Property::LineCount);
        propertyHash.insert(QStringLiteral("language"), Property::Langauge);
        propertyHash.insert(QStringLiteral("copyright"), Property::Copyright);
        propertyHash.insert(QStringLiteral("publisher"), Property::Publisher);
        propertyHash.insert(QStringLiteral("creationdate"), Property::CreationDate);
        propertyHash.insert(QStringLiteral("keywords"), Property::Keywords);
        propertyHash.insert(QStringLiteral("width"), Property::Width);
        propertyHash.insert(QStringLiteral("height"), Property::Height);
        propertyHash.insert(QStringLiteral("aspectratio"), Property::AspectRatio);
        propertyHash.insert(QStringLiteral("framerate"), Property::FrameRate);
        propertyHash.insert(QStringLiteral("imagemake"), Property::ImageMake);
        propertyHash.insert(QStringLiteral("imagemodel"), Property::ImageModel);
        propertyHash.insert(QStringLiteral("imagedatetime"), Property::ImageDateTime);
        propertyHash.insert(QStringLiteral("imageorientation"), Property::ImageOrientation);
        propertyHash.insert(QStringLiteral("photoflash"), Property::PhotoFlash);
        propertyHash.insert(QStringLiteral("photopixelxdimension"), Property::PhotoPixelXDimension);
        propertyHash.insert(QStringLiteral("photopixelydimension"), Property::PhotoPixelYDimension);
        propertyHash.insert(QStringLiteral("photodatetimeoriginal"), Property::PhotoDateTimeOriginal);
        propertyHash.insert(QStringLiteral("photofocallength"), Property::PhotoFocalLength);
        propertyHash.insert(QStringLiteral("photofocallengthin35mmfilm"), Property::PhotoFocalLengthIn35mmFilm);
        propertyHash.insert(QStringLiteral("photoexposuretime"), Property::PhotoExposureTime);
        propertyHash.insert(QStringLiteral("photofnumber"), Property::PhotoFNumber);
        propertyHash.insert(QStringLiteral("photoaperturevalue"), Property::PhotoApertureValue);
        propertyHash.insert(QStringLiteral("photoexposurebiasvalue"), Property::PhotoExposureBiasValue);
        propertyHash.insert(QStringLiteral("photowhitebalance"), Property::PhotoWhiteBalance);
        propertyHash.insert(QStringLiteral("photometeringmode"), Property::PhotoMeteringMode);
        propertyHash.insert(QStringLiteral("photoisospeedratings"), Property::PhotoISOSpeedRatings);
        propertyHash.insert(QStringLiteral("photosaturation"), Property::PhotoSaturation);
        propertyHash.insert(QStringLiteral("photosharpness"), Property::PhotoSharpness);
    }

    return PropertyInfo(propertyHash.value(name.toLower()));
}
