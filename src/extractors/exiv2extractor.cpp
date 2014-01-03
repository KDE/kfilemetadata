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

#include <KDebug>

using namespace KFileMetaData;


Exiv2Extractor::Exiv2Extractor(QObject* parent, const QVariantList&)
    : ExtractorPlugin(parent)
{

}

QStringList Exiv2Extractor::mimetypes()
{
    QStringList types;

    types << QLatin1String("image/jp2")
          << QLatin1String("image/jpeg")
          << QLatin1String("image/pgf")
          << QLatin1String("image/png")
          << QLatin1String("image/tiff")
          << QLatin1String("image/x-exv")
          << QLatin1String("image/x-canon-cr2")
          << QLatin1String("image/x-canon-crw")
          << QLatin1String("image/x-fuji-raf")
          << QLatin1String("image/x-minolta-mrw")
          << QLatin1String("image/x-nikon-nef")
          << QLatin1String("image/x-olympus-orf")
          << QLatin1String("image/x-panasonic-rw2")
          << QLatin1String("image/x-pentax-pef")
          << QLatin1String("image/x-photoshop")
          << QLatin1String("image/x-samsung-srw");

    return types;
}

namespace
{
QString toString(const Exiv2::Value& value)
{
    std::string str = value.toString();
    return QString::fromUtf8(str.c_str(), str.length());
}

QVariant toVariantDateTime(const Exiv2::Value& value)
{
    if (value.typeId() == Exiv2::asciiString) {
        QDateTime val = ExtractorPlugin::dateTimeFromString(value.toString().c_str());
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
    result->addType("Image");

    if (image->pixelHeight()) {
        result->add("height", image->pixelHeight());
    }

    if (image->pixelWidth()) {
        result->add("width", image->pixelWidth());
    }

    std::string comment = image->comment();
    if (!comment.empty()) {
        result->add("comment", QString::fromUtf8(comment.c_str(), comment.length()));
    }

    const Exiv2::ExifData& data = image->exifData();

    add(result, data, "Exif.Image.Make", QVariant::String);
    add(result, data, "Exif.Image.Model", QVariant::String);
    add(result, data, "Exif.Image.DateTime", QVariant::DateTime);
    add(result, data, "Exif.Image.Orientation", QVariant::Int);
    add(result, data, "Exif.Photo.Flash", QVariant::Int);
    add(result, data, "Exif.Photo.PixelXDimension", QVariant::Int);
    add(result, data, "Exif.Photo.PixelYDimension", QVariant::Int);
    add(result, data, "Exif.Photo.DateTimeOriginal", QVariant::DateTime);
    add(result, data, "Exif.Photo.FocalLength", QVariant::Double);
    add(result, data, "Exif.Photo.FocalLengthIn35mmFilm", QVariant::Double);
    add(result, data, "Exif.Photo.ExposureTime", QVariant::Double);
    add(result, data, "Exif.Photo.FNumber", QVariant::Double);
    add(result, data, "Exif.Photo.ApertureValue", QVariant::Double);
    add(result, data, "Exif.Photo.ExposureBiasValue", QVariant::Double);
    add(result, data, "Exif.Photo.WhiteBalance", QVariant::Int);
    add(result, data, "Exif.Photo.MeteringMode", QVariant::Int);
    add(result, data, "Exif.Photo.ISOSpeedRatings", QVariant::Int);
    add(result, data, "Exif.Photo.Saturation", QVariant::Int);
    add(result, data, "Exif.Photo.Sharpness", QVariant::Int);
}

void Exiv2Extractor::add(ExtractionResult* result, const Exiv2::ExifData& data,
                         const char* name, QVariant::Type type)
{
    Exiv2::ExifData::const_iterator it = data.findKey(Exiv2::ExifKey(name));
    if (it != data.end()) {
        QVariant value = toVariant(it->value(), type);
        if (!value.isNull())
            result->add(name, value);
    }
}

KFILEMETADATA_EXPORT_EXTRACTOR(KFileMetaData::Exiv2Extractor, "kmetaddata_exivextractor")
