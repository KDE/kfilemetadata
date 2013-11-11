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

#include "nfo.h"
#include "nexif.h"
#include "nie.h"

#include <KDebug>

#include <exiv2/exiv2.hpp>

using namespace Nepomuk2::Vocabulary;

namespace Nepomuk2
{

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

QVariant toVariantFloat(const Exiv2::Value& value)
{
    // WARNING: Dbus does not recognize float, cast to double
    if (value.typeId() == Exiv2::tiffFloat || value.typeId() == Exiv2::tiffDouble || value.typeId() == Exiv2::unsignedRational || value.typeId() == Exiv2::signedRational)
        return QVariant(static_cast<double>(value.toFloat()));

    QString str(toString(value));
    bool ok = false;
    double val = str.toFloat(&ok);
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
}

SimpleResourceGraph Exiv2Extractor::extract(const QUrl& resUri, const QUrl& fileUrl, const QString& mimeType)
{
    Q_UNUSED(mimeType);

    QByteArray arr = fileUrl.toLocalFile().toUtf8();
    std::string fileString(arr.data(), arr.length());

    Exiv2::Image::AutoPtr image;
    try {
        image = Exiv2::ImageFactory::open(fileString);
    } catch (const std::exception&) {
        return SimpleResourceGraph();
    }
    if (!image.get()) {
        return SimpleResourceGraph();
    }

    try {
        image->readMetadata();
    } catch (const std::exception&) {
        return SimpleResourceGraph();
    }
    const Exiv2::ExifData& data = image->exifData();

    SimpleResourceGraph graph;
    SimpleResource fileRes(resUri);
    fileRes.addType(NFO::RasterImage());

    if (image->pixelHeight()) {
        fileRes.setProperty(NFO::height(), image->pixelHeight());
    }

    if (image->pixelWidth()) {
        fileRes.setProperty(NFO::width(), image->pixelWidth());
    }

    std::string comment = image->comment();
    if (!comment.empty()) {
        fileRes.setProperty(NIE::comment(), QString::fromUtf8(comment.c_str(), comment.length()));
    }

    /*
    Exiv2::ExifData::const_iterator end = data.end();
    Exiv2::ExifData::const_iterator i = data.begin();
    for( ; i != end; i++ ) {
        kDebug() << i->key().c_str();
        kDebug() << i->value().toString().c_str();
    }*/

    Exiv2::ExifData::const_iterator it;

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.Flash"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::flash(), value);
    }

    // The width and height have already been set above, this is not required
    /*it = data.findKey( Exiv2::ExifKey("Exif.Photo.PixelXDimension") );
    if( it != data.end() ) {
        QVariant value = toVariantLong( it->value() );
        if( !value.isNull() )
            fileRes.setProperty( NFO::width(), value );
    }

    it = data.findKey( Exiv2::ExifKey("Exif.Photo.PixelYDimension") );
    if( it != data.end() ) {
        QVariant value = toVariantLong( it->value() );
        if( !value.isNull() )
            fileRes.setProperty( NFO::height(), value );
    }*/

    it = data.findKey(Exiv2::ExifKey("Exif.Image.Make"));
    if (it != data.end()) {
        QVariant value = toVariantString(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::make(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Image.Model"));
    if (it != data.end()) {
        QVariant value = toVariantString(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::model(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Image.DateTime"));
    if (it != data.end()) {
        QVariant value = toVariantDateTime(it->value());
        if (!value.isNull())
            fileRes.setProperty(NIE::contentCreated(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::orientation(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.DateTimeOriginal"));
    if (it != data.end()) {
        QVariant value = toVariantDateTime(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::dateTimeOriginal(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength"));
    if (it != data.end()) {
        QVariant value = toVariantFloat(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::focalLength(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.FocalLengthIn35mmFilm"));
    if (it != data.end()) {
        QVariant value = toVariantFloat(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::focalLengthIn35mmFilm(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.ExposureTime"));
    if (it != data.end()) {
        QVariant value = toVariantFloat(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::exposureTime(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.FNumber"));
    if (it != data.end()) {
        QVariant value = toVariantFloat(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::fNumber(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.ApertureValue"));
    if (it != data.end()) {
        QVariant value = toVariantFloat(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::apertureValue(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.ExposureBiasValue"));
    if (it != data.end()) {
        QVariant value = toVariantFloat(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::exposureBiasValue(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.WhiteBalance"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::whiteBalance(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.MeteringMode"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::meteringMode(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.ISOSpeedRatings"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::isoSpeedRatings(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.Saturation"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::saturation(), value);
    }

    it = data.findKey(Exiv2::ExifKey("Exif.Photo.Sharpness"));
    if (it != data.end()) {
        QVariant value = toVariantLong(it->value());
        if (!value.isNull())
            fileRes.setProperty(NEXIF::sharpness(), value);
    }

    fileRes.addType(NEXIF::Photo());

    graph << fileRes;
    return graph;
}

}

NEPOMUK_EXPORT_EXTRACTOR(Nepomuk2::Exiv2Extractor, "nepomukexivextractor")
