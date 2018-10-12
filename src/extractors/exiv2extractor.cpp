/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>

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


#include "exiv2extractor.h"
#include <QDebug>

using namespace KFileMetaData;


Exiv2Extractor::Exiv2Extractor(QObject* parent)
    : ExtractorPlugin(parent)
{
}

QStringList Exiv2Extractor::mimetypes() const
{
    QStringList types;

    types << QStringLiteral("image/jp2")
          << QStringLiteral("image/jpeg")
          << QStringLiteral("image/pgf")
          << QStringLiteral("image/png")
          << QStringLiteral("image/tiff")
          << QStringLiteral("image/x-exv")
          << QStringLiteral("image/x-canon-cr2")
          << QStringLiteral("image/x-canon-crw")
          << QStringLiteral("image/x-fuji-raf")
          << QStringLiteral("image/x-minolta-mrw")
          << QStringLiteral("image/x-nikon-nef")
          << QStringLiteral("image/x-olympus-orf")
          << QStringLiteral("image/x-panasonic-rw2")
          << QStringLiteral("image/x-pentax-pef")
          << QStringLiteral("image/x-photoshop")
          << QStringLiteral("image/x-samsung-srw");

    return types;
}

namespace
{
QString toString(const Exiv2::Value& value)
{
    const std::string str = value.toString();
    return QString::fromUtf8(str.c_str(), str.length());
}

QVariant toVariantDateTime(const Exiv2::Value& value)
{
    if (value.typeId() == Exiv2::asciiString) {
        QDateTime val = ExtractorPlugin::dateTimeFromString(QString::fromLatin1(value.toString().c_str()));
        if (val.isValid()) {
            // Datetime is stored in exif as local time.
            val.setUtcOffset(0);
            return QVariant(val);
        }
    }

    return QVariant();
}

QVariant toVariantLong(const Exiv2::Value& value)
{
    if (value.typeId() == Exiv2::unsignedLong || value.typeId() == Exiv2::signedLong) {
        qlonglong val = value.toLong();
        return QVariant(val);
    }

    QString str(toString(value));
    bool ok = false;
    int val = str.toInt(&ok);
    if (ok)
        return QVariant(val);

    return QVariant();
}

QVariant toVariantDouble(const Exiv2::Value& value)
{
    if (value.typeId() == Exiv2::tiffFloat || value.typeId() == Exiv2::tiffDouble
        || value.typeId() == Exiv2::unsignedRational || value.typeId() == Exiv2::signedRational) {
        return QVariant(static_cast<double>(value.toFloat()));
    }

    QString str(toString(value));
    bool ok = false;
    double val = str.toDouble(&ok);
    if (ok)
        return QVariant(val);

    return QVariant();
}

QVariant toVariantString(const Exiv2::Value& value)
{
    QString str = toString(value);
    if (!str.isEmpty())
        return QVariant(str);

    return QVariant();
}

QVariant toVariant(const Exiv2::Value& value, QVariant::Type type) {
    if (value.count() == 0) {
        return QVariant();
    }
    switch (type) {
    case QVariant::Int:
        return toVariantLong(value);

    case QVariant::DateTime:
        return toVariantDateTime(value);

    case QVariant::Double:
        return toVariantDouble(value);

    case QVariant::String:
    default:
        return toVariantString(value);
    }
}
}

void Exiv2Extractor::extract(ExtractionResult* result)
{
    QByteArray arr = result->inputUrl().toUtf8();
    std::string fileString(arr.data(), arr.length());

    Exiv2::Image::AutoPtr image;
    try {
        image = Exiv2::ImageFactory::open(fileString);
    } catch (const std::exception&) {
        return;
    }
    if (!image.get()) {
        return;
    }

    try {
        image->readMetadata();
    } catch (const std::exception&) {
        return;
    }
    result->addType(Type::Image);

    if (image->pixelHeight()) {
        result->add(Property::Height, image->pixelHeight());
    }

    if (image->pixelWidth()) {
        result->add(Property::Width, image->pixelWidth());
    }

    std::string comment = image->comment();
    if (!comment.empty()) {
        result->add(Property::Comment, QString::fromUtf8(comment.c_str(), comment.length()));
    }

    const Exiv2::ExifData& data = image->exifData();

    add(result, data, Property::ImageMake, "Exif.Image.Make", QVariant::String);
    add(result, data, Property::ImageModel, "Exif.Image.Model", QVariant::String);
    add(result, data, Property::ImageDateTime, "Exif.Image.DateTime", QVariant::DateTime);
    add(result, data, Property::ImageOrientation, "Exif.Image.Orientation", QVariant::Int);
    add(result, data, Property::PhotoFlash, "Exif.Photo.Flash", QVariant::Int);
    add(result, data, Property::PhotoPixelXDimension, "Exif.Photo.PixelXDimension", QVariant::Int);
    add(result, data, Property::PhotoPixelXDimension, "Exif.Photo.PixelYDimension", QVariant::Int);
    add(result, data, Property::PhotoDateTimeOriginal, "Exif.Photo.DateTimeOriginal", QVariant::DateTime);
    add(result, data, Property::PhotoFocalLength, "Exif.Photo.FocalLength", QVariant::Double);
    add(result, data, Property::PhotoFocalLengthIn35mmFilm, "Exif.Photo.FocalLengthIn35mmFilm", QVariant::Double);
    add(result, data, Property::PhotoExposureTime, "Exif.Photo.ExposureTime", QVariant::Double);
    add(result, data, Property::PhotoExposureBiasValue, "Exif.Photo.ExposureBiasValue", QVariant::Double);
    add(result, data, Property::PhotoFNumber, "Exif.Photo.FNumber", QVariant::Double);
    add(result, data, Property::PhotoApertureValue, "Exif.Photo.ApertureValue", QVariant::Double);
    add(result, data, Property::PhotoWhiteBalance, "Exif.Photo.WhiteBalance", QVariant::Int);
    add(result, data, Property::PhotoMeteringMode, "Exif.Photo.MeteringMode", QVariant::Int);
    add(result, data, Property::PhotoISOSpeedRatings, "Exif.Photo.ISOSpeedRatings", QVariant::Int);
    add(result, data, Property::PhotoSaturation, "Exif.Photo.Saturation", QVariant::Int);
    add(result, data, Property::PhotoSharpness, "Exif.Photo.Sharpness", QVariant::Int);

    double latitude = fetchGpsDouble(data, "Exif.GPSInfo.GPSLatitude");
    double longitude = fetchGpsDouble(data, "Exif.GPSInfo.GPSLongitude");
    double altitude = fetchGpsDouble(data, "Exif.GPSInfo.GPSAltitude");

    QByteArray latRef = fetchByteArray(data, "Exif.GPSInfo.GPSLatitudeRef");
    if (!latRef.isEmpty() && latRef[0] == 'S')
        latitude *= -1;

    QByteArray longRef = fetchByteArray(data, "Exif.GPSInfo.GPSLongitudeRef");
    if (!longRef.isEmpty() && longRef[0] == 'W')
        longitude *= -1;

    if (latitude) {
        result->add(Property::PhotoGpsLatitude, latitude);
    }

    if (longitude) {
        result->add(Property::PhotoGpsLongitude, longitude);
    }

    if (altitude) {
        result->add(Property::PhotoGpsAltitude, altitude);
    }
}

void Exiv2Extractor::add(ExtractionResult* result, const Exiv2::ExifData& data,
                         Property::Property prop, const char* name,
                         QVariant::Type type)
{
    Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey(name));
    if (it != data.end()) {
        QVariant value = toVariant(it->value(), type);
        if (!value.isNull())
            result->add(prop, value);
    }
}

double Exiv2Extractor::fetchGpsDouble(const Exiv2::ExifData& data, const char* name)
{
    Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey(name));
    if (it != data.end() && it->count() == 3) {
        double n = 0.0;
        double d = 0.0;

        n = (*it).toRational(0).first;
        d = (*it).toRational(0).second;

        if (d == 0) {
            return 0.0;
        }

        double deg = n / d;

        n = (*it).toRational(1).first;
        d = (*it).toRational(1).second;

        if (d == 0) {
            return deg;
        }

        double min = n / d;
        if (min != -1.0) {
            deg += min / 60.0;
        }

        n = (*it).toRational(2).first;
        d = (*it).toRational(2).second;

        if (d == 0) {
            return deg;
        }

        double sec = n / d;
        if (sec != -1.0) {
            deg += sec / 3600.0;
        }

        return deg;
    }

    return 0.0;
}

QByteArray Exiv2Extractor::fetchByteArray(const Exiv2::ExifData& data, const char* name)
{
    Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey(name));
    if (it != data.end()) {
        std::string str = it->value().toString();
        return QByteArray(str.c_str(), str.size());
    }

    return QByteArray();
}
