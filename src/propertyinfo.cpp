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
            d->displayName = i18nc("@label", "Genre");
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
            d->displayName = i18nc("@label", "Image Date Time");
            d->valueType = QVariant::DateTime;
            break;

        case Property::ImageMake:
            d->name = QLatin1String("imageMake");
            d->displayName = i18nc("@label", "Image Make");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::ImageModel:
            d->name = QLatin1String("imageModel");
            d->displayName = i18nc("@label", "Image Model");
            d->valueType = QVariant::String;
            d->shouldBeIndexed = false;
            break;

        case Property::ImageOrientation:
            d->name = QLatin1String("imageOrientation");
            d->displayName = i18nc("@label", "Image Orientation");
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
            d->displayName = i18nc("@label", "Line Count");
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
            d->displayName = i18nc("@label", "Photo Aperture Value");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoDateTimeOriginal:
            d->name = QLatin1String("photoDateTimeOriginal");
            d->displayName = i18nc("@label", "Photo Original Date Time");
            d->valueType = QVariant::DateTime;
            break;

        case Property::PhotoExposureBiasValue:
            d->name = QLatin1String("photoExposureBiasValue");
            d->displayName = i18nc("@label", "Photo Exposure Bias");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoExposureTime:
            d->name = QLatin1String("photoExposureTime");
            d->displayName = i18nc("@label", "Photo Exposure Time");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoFlash:
            d->name = QLatin1String("photoFlash");
            d->displayName = i18nc("@label", "Photo Flash");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoFNumber:
            d->name = QLatin1String("photoFNumber");
            d->displayName = i18nc("@label", "Photo F Number");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoFocalLength:
            d->name = QLatin1String("photoFocalLength");
            d->displayName = i18nc("@label", "Photo Focal Length");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoFocalLengthIn35mmFilm:
            d->name = QLatin1String("photoFocalLengthIn35mmFilm");
            d->displayName = i18nc("@label", "Photo Focal Length 35mm");
            d->valueType = QVariant::Double;
            break;

        case Property::PhotoISOSpeedRatings:
            d->name = QLatin1String("photoISOSpeedRatings");
            d->displayName = i18nc("@label", "Photo ISO Speed Rating");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoMeteringMode:
            d->name = QLatin1String("photoMeteringMode");
            d->displayName = i18nc("@label", "Photo Metering Mode");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoPixelXDimension:
            d->name = QLatin1String("photoPixelXDimension");
            d->displayName = i18nc("@label", "Photo X Dimension");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoPixelYDimension:
            d->name = QLatin1String("photoPixelYDimension");
            d->displayName = i18nc("@label", "Photo Y Dimension");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoSaturation:
            d->name = QLatin1String("photoSaturation");
            d->displayName = i18nc("@label", "Photo Saturation");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoSharpness:
            d->name = QLatin1String("photoSharpness");
            d->displayName = i18nc("@label", "Photo Sharpness");
            d->valueType = QVariant::Int;
            break;

        case Property::PhotoWhiteBalance:
            d->name = QLatin1String("photoWhiteBalance");
            d->displayName = i18nc("@label", "Photo White Balance");
            d->valueType = QVariant::Int;
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
            d->name = QLatin1String("Title");
            d->displayName = i18nc("@label", "Title");
            d->valueType = QVariant::String;
            break;

        case Property::TrackNumber:
            d->name = QLatin1String("trackNumber");
            d->displayName = i18nc("@label", "Track Number");
            d->valueType = QVariant::Int;
            break;

        case Property::Width:
            d->name = QLatin1String("width");
            d->displayName = i18nc("@label", "Width");
            d->valueType = QVariant::Int;
            break;

        case Property::WordCount:
            d->name = QLatin1String("wordCount");
            d->displayName = i18nc("@label", "Word Count");
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
