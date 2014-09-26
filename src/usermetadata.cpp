/*
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "usermetadata.h"
#include "xattr_p.h"

#include <QSet>

using namespace KFileMetaData;

class UserMetaData::Private {
public:
    QString filePath;
};

UserMetaData::UserMetaData(const QString& filePath)
    : d(new Private)
{
    d->filePath = filePath;
}

UserMetaData::UserMetaData(const UserMetaData& rhs)
    : d(new Private(*rhs.d))
{
}

UserMetaData::~UserMetaData()
{
    delete d;
}

const UserMetaData& UserMetaData::operator=(const UserMetaData& rhs)
{
    d->filePath = rhs.d->filePath;
    return *this;
}

QString UserMetaData::filePath() const
{
    return d->filePath;
}

void UserMetaData::addTag(const QString& tag)
{
    QSet<QString> tagSet = tags().toSet();
    tagSet << tag;

    setTags(tagSet.toList());
}

void UserMetaData::addTags(const QStringList& tagList)
{
    QSet<QString> tagSet = tags().toSet();
    tagSet.unite(tagList.toSet());

    setTags(tagSet.toList());
}

void UserMetaData::setTags(const QStringList& tags)
{
    k_setxattr(d->filePath, QStringLiteral("user.xdg.tags"), tags.join(", "));
}

void UserMetaData::removeTag(const QString& tag)
{
    QSet<QString> tagSet = tags().toSet();
    tagSet.remove(tag);

    setTags(tagSet.toList());
}

void UserMetaData::removeTags(const QStringList& tagList)
{
    QSet<QString> tagSet = tags().toSet();
    tagSet.subtract(tagList.toSet());

    setTags(tagSet.toList());

}

QStringList UserMetaData::tags() const
{
    QString value;

    k_getxattr(d->filePath, QStringLiteral("user.xdg.tags"), &value);
    return value.split(QLatin1Char(','), QString::SkipEmptyParts);
}

int UserMetaData::rating() const
{
    QString value;

    k_getxattr(d->filePath, QStringLiteral("user.baloo.rating"), &value);
    return value.toInt();
}

void UserMetaData::setRating(int rating)
{
    k_setxattr(d->filePath, QStringLiteral("user.baloo.rating"), QString::number(rating));
}

QString UserMetaData::userComment() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.xdg.comment"), &value);

    return value;
}

void UserMetaData::setUserComment(const QString& userComment)
{
    k_setxattr(d->filePath, QStringLiteral("user.xdg.comment"), userComment);
}

void UserMetaData::setAttribute(const QString& key, const QString& value)
{
    k_setxattr(d->filePath, QStringLiteral("user") + key, value);
}

bool UserMetaData::hasAttribute(const QString& key)
{
    k_getxattr(d->filePath, QStringLiteral("user") + key, 0);

    return (errno != ENODATA);
}

QString UserMetaData::attribute(const QString& key)
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user") + key, &value);

    return value;
}

bool UserMetaData::isSupported() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.test"), &value);

    return (errno == ENOTSUP);
}
