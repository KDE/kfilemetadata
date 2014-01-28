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
#include <KGlobal>

static const KCatalogLoader loader("kfilemetadata");

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
            d->name = QLatin1String("album");
            d->displayName = i18nc("@label music album", "Album");
            d->valueType = QVariant::String;
            break;

        case Property::AlbumArtist:
            d->name = QLatin1String("albumArtist");
            d->displayName = i18nc("@label", "Album Artist");
            d->valueType = QVariant::StringList;
            break;

        case Property::Artist:
            d->name = QLatin1String("artist");
            d->displayName = i18nc("@label", "Artist");
            d->valueType = QVariant::StringList;
            break;

        case Property::AspectRatio:
            d->name = QLatin1String("aspectRatio");
            d->displayName = i18nc("@label", "Aspect Ratio");
            d->valueType = QVariant::Int;
            break;

        case Property::Author:
            d->name = QLatin1String("author");
            d->displayName = i18nc("@label", "Author");
            d->valueType = QVariant::StringList;
            break;

        case Property::BitRate:
            d->name = QLatin1String("bitRate");
            d->displayName = i18nc("@label", "Bitrate");
            d->valueType = QVariant::Int;
            break;

        case Property::Channels:
            d->name = QLatin1String("channels");
            d->displayName = i18nc("@label", "Channels");
            d->valueType = QVariant::Int;
            break;

        case Property::Comment:
            d->name = QLatin1String("comment");
            d->displayName = i18nc("@label", "Comment");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Composer:
            d->name = QLatin1String("composer");
            d->displayName = i18nc("@label", "Composer");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Copyright:
            d->name = QLatin1String("copyright");
            d->displayName = i18nc("@label", "Copyright");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::CreationDate:
            d->name = QLatin1String("creationDate");
            d->displayName = i18nc("@label", "Creation Date");
            d->valueType = QVariant::String;
            break;

        case Property::Creator:
            d->name = QLatin1String("creator");
            d->displayName = i18nc("@label", "Creator");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Description:
            d->name = QLatin1String("description");
            d->displayName = i18nc("@label", "Description");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Duration:
            d->name = QLatin1String("duration");
            d->displayName = i18nc("@label", "Duration");
            d->valueType = QVariant::Int;
            break;

        case Property::Empty:
            d->name = QLatin1String("empty");
            d->valueType = QVariant::Invalid;
            break;

        case Property::FrameRate:
            d->name = QLatin1String("frameRate");
            d->displayName = i18nc("@label", "Frame Rate");
            d->valueType = QVariant::Int;
            break;

        // FIXME: This doesn't tell the user much
        case Property::Generator:
            d->name = QLatin1String("generator");
            d->displayName = i18nc("@label", "Generator");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Genre:
            d->name = QLatin1String("genre");
            d->displayName = i18nc("@label music genre", "Genre");
            d->valueType = QVariant::StringList;
            d->shouldBeIndexed = false;
            break;

        case Property::Height:
            d->name = QLatin1String("height");
            d->displayName = i18nc("@label", "Height");
            d->valueType = QVariant::Int;
            break;

        case Property::ImageDateTime:
            d->name = QLatin1String("imageDateTime");
            d->displayName = i18nc("@label EXIF", "Image Date Time");
            d->valueType = QVariant::DateTime;
            break;

        case Property::ImageMake:
            d->name = QLatin1String("imageMake");
            d->displayName = i18nc("@label EXIF", "Image Make");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::ImageModel:
            d->name = QLatin1String("imageModel");
            d->displayName = i18nc("@label EXIF", "Image Model");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::ImageOrientation:
            d->name = QLatin1String("imageOrientation");
            d->displayName = i18nc("@label EXIF", "Image Orientation");
            d->valueType = QVariant::Int;
            break;

        case Property::Keywords:
            d->name = QLatin1String("keywords");
            d->displayName = i18nc("@label", "Keywords");
            d->valueType = QVariant::StringList;
            d->shouldBeIndexed = false;
            break;

        case Property::Langauge:
            d->name = QLatin1String("language");
            d->displayName = i18nc("@label", "Language");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::LineCount:
            d->name = QLatin1String("lineCount");
            d->displayName = i18nc("@label number of lines", "Line Count");
            d->valueType = QVariant::Int;
            break;

        case Property::Lyricist:
            d->name = QLatin1String("lyricist");
            d->displayName = i18nc("@label", "Lyricist");
            d->valueType = QVariant::StringList;
            d->shouldBeIndexed = false;
            break;

        case Property::PageCount:
            d->name = QLatin1String("pageCount");
            d->displayName = i18nc("@label", "Page Count");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoApertureValue:
            d->name = QLatin1String("photoApertureValue");
            d->displayName = i18nc("@label EXIF", "Photo Aperture Value");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoDateTimeOriginal:
            d->name = QLatin1String("photoDateTimeOriginal");
            d->displayName = i18nc("@label EXIF", "Photo Original Date Time");
            d->valueType = QVariant::DateTime;
            break;

        case Property::PhotoExposureBiasValue:
            d->name = QLatin1String("photoExposureBiasValue");
            d->displayName = i18nc("@label EXIF", "Photo Exposure Bias");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoExposureTime:
            d->name = QLatin1String("photoExposureTime");
            d->displayName = i18nc("@label EXIF", "Photo Exposure Time");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoFlash:
            d->name = QLatin1String("photoFlash");
            d->displayName = i18nc("@label EXIF", "Photo Flash");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoFNumber:
            d->name = QLatin1String("photoFNumber");
            d->displayName = i18nc("@label EXIF", "Photo F Number");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoFocalLength:
            d->name = QLatin1String("photoFocalLength");
            d->displayName = i18nc("@label EXIF", "Photo Focal Length");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoFocalLengthIn35mmFilm:
            d->name = QLatin1String("photoFocalLengthIn35mmFilm");
            d->displayName = i18nc("@label EXIF", "Photo Focal Length 35mm");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoISOSpeedRatings:
            d->name = QLatin1String("photoISOSpeedRatings");
            d->displayName = i18nc("@label EXIF", "Photo ISO Speed Rating");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoMeteringMode:
            d->name = QLatin1String("photoMeteringMode");
            d->displayName = i18nc("@label EXIF", "Photo Metering Mode");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoPixelXDimension:
            d->name = QLatin1String("photoPixelXDimension");
            d->displayName = i18nc("@label EXIF", "Photo X Dimension");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoPixelYDimension:
            d->name = QLatin1String("photoPixelYDimension");
            d->displayName = i18nc("@label EXIF", "Photo Y Dimension");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoSaturation:
            d->name = QLatin1String("photoSaturation");
            d->displayName = i18nc("@label EXIF", "Photo Saturation");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoSharpness:
            d->name = QLatin1String("photoSharpness");
            d->displayName = i18nc("@label EXIF", "Photo Sharpness");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoWhiteBalance:
            d->name = QLatin1String("photoWhiteBalance");
            d->displayName = i18nc("@label EXIF", "Photo White Balance");
            d->valueType = QVariant::Int;
            break;

        case Property::Publisher:
            d->name = QLatin1String("publisher");
            d->displayName = i18nc("@label", "Publisher");
            d->valueType = QVariant::String;
            break;

        case Property::ReleaseYear:
            d->name = QLatin1String("releaseYear");
            d->displayName = i18nc("@label", "Release Year");
            d->valueType = QVariant::Int;
            break;

        case Property::SampleRate:
            d->name = QLatin1String("sampleRate");
            d->displayName = i18nc("@label", "Sample Rate");
            d->valueType = QVariant::Int;
            break;

        case Property::Subject:
            d->name = QLatin1String("subject");
            d->displayName = i18nc("@label", "Subject");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::Title:
            d->name = QLatin1String("title");
            d->displayName = i18nc("@label", "Title");
            d->valueType = QVariant::String;
            break;

        case Property::TrackNumber:
            d->name = QLatin1String("trackNumber");
            d->displayName = i18nc("@label music track number", "Track Number");
            d->valueType = QVariant::Int;
            break;

        case Property::Width:
            d->name = QLatin1String("width");
            d->displayName = i18nc("@label", "Width");
            d->valueType = QVariant::Int;
            break;

        case Property::WordCount:
            d->name = QLatin1String("wordCount");
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
        propertyHash.insert(QLatin1String("bitrate"), Property::BitRate);
        propertyHash.insert(QLatin1String("channels"), Property::Channels);
        propertyHash.insert(QLatin1String("duration"), Property::Duration);
        propertyHash.insert(QLatin1String("genre"), Property::Genre);
        propertyHash.insert(QLatin1String("samplerate"), Property::SampleRate);
        propertyHash.insert(QLatin1String("tracknumber"), Property::TrackNumber);
        propertyHash.insert(QLatin1String("releaseyear"), Property::ReleaseYear);
        propertyHash.insert(QLatin1String("comment"), Property::Comment);
        propertyHash.insert(QLatin1String("artist"), Property::Artist);
        propertyHash.insert(QLatin1String("album"), Property::Album);
        propertyHash.insert(QLatin1String("albumartist"), Property::AlbumArtist);
        propertyHash.insert(QLatin1String("composer"), Property::Composer);
        propertyHash.insert(QLatin1String("lyricist"), Property::Lyricist);
        propertyHash.insert(QLatin1String("author"), Property::Author);
        propertyHash.insert(QLatin1String("title"), Property::Title);
        propertyHash.insert(QLatin1String("subject"), Property::Subject);
        propertyHash.insert(QLatin1String("creator"), Property::Creator);
        propertyHash.insert(QLatin1String("generator"), Property::Generator);
        propertyHash.insert(QLatin1String("pagecount"), Property::PageCount);
        propertyHash.insert(QLatin1String("wordcount"), Property::WordCount);
        propertyHash.insert(QLatin1String("linecount"), Property::LineCount);
        propertyHash.insert(QLatin1String("language"), Property::Langauge);
        propertyHash.insert(QLatin1String("copyright"), Property::Copyright);
        propertyHash.insert(QLatin1String("publisher"), Property::Publisher);
        propertyHash.insert(QLatin1String("description"), Property::Description);
        propertyHash.insert(QLatin1String("creationdate"), Property::CreationDate);
        propertyHash.insert(QLatin1String("keywords"), Property::Keywords);
        propertyHash.insert(QLatin1String("width"), Property::Width);
        propertyHash.insert(QLatin1String("height"), Property::Height);
        propertyHash.insert(QLatin1String("aspectratio"), Property::AspectRatio);
        propertyHash.insert(QLatin1String("framerate"), Property::FrameRate);
        propertyHash.insert(QLatin1String("imagemake"), Property::ImageMake);
        propertyHash.insert(QLatin1String("imagemodel"), Property::ImageModel);
        propertyHash.insert(QLatin1String("imagedatetime"), Property::ImageDateTime);
        propertyHash.insert(QLatin1String("imageorientation"), Property::ImageOrientation);
        propertyHash.insert(QLatin1String("photoflash"), Property::PhotoFlash);
        propertyHash.insert(QLatin1String("photopixelxdimension"), Property::PhotoPixelXDimension);
        propertyHash.insert(QLatin1String("photopixelydimension"), Property::PhotoPixelYDimension);
        propertyHash.insert(QLatin1String("photodatetimeoriginal"), Property::PhotoDateTimeOriginal);
        propertyHash.insert(QLatin1String("photofocallength"), Property::PhotoFocalLength);
        propertyHash.insert(QLatin1String("photofocallengthin35mmfilm"), Property::PhotoFocalLengthIn35mmFilm);
        propertyHash.insert(QLatin1String("photoexposuretime"), Property::PhotoExposureTime);
        propertyHash.insert(QLatin1String("photofnumber"), Property::PhotoFNumber);
        propertyHash.insert(QLatin1String("photoaperturevalue"), Property::PhotoApertureValue);
        propertyHash.insert(QLatin1String("photoexposurebiasvalue"), Property::PhotoExposureBiasValue);
        propertyHash.insert(QLatin1String("photowhitebalance"), Property::PhotoWhiteBalance);
        propertyHash.insert(QLatin1String("photometeringmode"), Property::PhotoMeteringMode);
        propertyHash.insert(QLatin1String("photoisospeedratings"), Property::PhotoISOSpeedRatings);
        propertyHash.insert(QLatin1String("photosaturation"), Property::PhotoSaturation);
        propertyHash.insert(QLatin1String("photosharpness"), Property::PhotoSharpness);
    }

    return PropertyInfo(propertyHash.value(name.toLower()));
}
