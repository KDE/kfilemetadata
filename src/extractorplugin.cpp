/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Vishesh Handa <me@vhanda.in>
    Copyright (C) 2012  Jörg Ehrichs <joerg.ehrichs@gmx.de>

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


#include "extractorplugin.h"

#include <QMimeDatabase>
#include <QDebug>

using namespace KFileMetaData;

ExtractorPlugin::ExtractorPlugin(QObject* parent): QObject(parent)
{
}

ExtractorPlugin::~ExtractorPlugin()
{
}

//
// Helper functions
//

QDateTime ExtractorPlugin::dateTimeFromString(const QString& dateString)
{
    QDateTime dateTime;

    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yyyy-MM-dd"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("dd-MM-yyyy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yyyy-MM"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("MM-yyyy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yyyy.MM.dd"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("dd.MM.yyyy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("dd MMMM yyyy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("MM.yyyy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yyyy.MM"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yyyy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yy"));
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, Qt::ISODate);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("dddd d MMM yyyy h':'mm':'ss AP"));
        dateTime.setTimeSpec(Qt::LocalTime);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, QStringLiteral("yyyy:MM:dd hh:mm:ss"));
        dateTime.setTimeSpec(Qt::LocalTime);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, Qt::SystemLocaleDate);
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, Qt::SystemLocaleShortDate);
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        dateTime = QDateTime::fromString(dateString, Qt::SystemLocaleLongDate);
        dateTime.setTimeSpec(Qt::UTC);
    }
    if (!dateTime.isValid()) {
        qWarning() << "Could not determine correct datetime format from:" << dateString;
        return QDateTime();
    }

    return dateTime;
}

QStringList ExtractorPlugin::contactsFromString(const QString& string)
{
    QString cleanedString = string;
    cleanedString = cleanedString.remove(QLatin1Char('{'));
    cleanedString = cleanedString.remove(QLatin1Char('}'));

    QStringList contactStrings = string.split(QLatin1Char(','), QString::SkipEmptyParts);
    if (contactStrings.size() == 1)
        contactStrings = string.split(QLatin1Char(';'), QString::SkipEmptyParts);

    if (contactStrings.size() == 1)
        contactStrings = string.split(QStringLiteral(" ft "), QString::SkipEmptyParts);

    if (contactStrings.size() == 1)
        contactStrings = string.split(QStringLiteral(" feat. "), QString::SkipEmptyParts);

    if (contactStrings.size() == 1)
        contactStrings = string.split(QStringLiteral(" feat "), QString::SkipEmptyParts);

    QStringList list;
    list.reserve(contactStrings.count());
    for (const QString& contactName : qAsConst(contactStrings)) {
        list << contactName.trimmed();
    }

    return list;
}

QString ExtractorPlugin::getSupportedMimeType(const QString& mimetype) const
{
    const QStringList allTypes = mimetypes();
    if (allTypes.contains(mimetype)) {
        return mimetype;
    }

    QMimeDatabase db;
    auto type = db.mimeTypeForName(mimetype);
    const QStringList ancestors = type.allAncestors();
    for (auto ancestor : ancestors) {
        if (allTypes.contains(ancestor)) {
            return ancestor;
        }
    }

    return QString();
}
