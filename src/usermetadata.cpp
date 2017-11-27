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

class Q_DECL_HIDDEN UserMetaData::Private
{
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

UserMetaData::Error UserMetaData::setTags(const QStringList& tags)
{
    if (!tags.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.xdg.tags"), tags.join(QLatin1Char(',')));
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.xdg.tags"));
    }

    return NoError;
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

UserMetaData::Error UserMetaData::setRating(int rating)
{
    if (rating) {
        k_setxattr(d->filePath, QStringLiteral("user.baloo.rating"), QString::number(rating));
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.baloo.rating"));
    }

    return NoError;
}

QString UserMetaData::userComment() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.xdg.comment"), &value);

    return value;
}

UserMetaData::Error UserMetaData::setUserComment(const QString& userComment)
{
    if (!userComment.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.xdg.comment"), userComment);
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.xdg.comment"));
    }

    return NoError;
}

QUrl UserMetaData::originUrl() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.xdg.origin.url"), &value);

    return QUrl(value);
}

UserMetaData::Error UserMetaData::setOriginUrl(const QUrl &originUrl)
{
    if (!originUrl.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.xdg.origin.url"), originUrl.toString());
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.xdg.origin.url"));
    }

    return NoError;
}

QString UserMetaData::originEmailSubject() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.xdg.origin.email.subject"), &value);
    return value;
}

UserMetaData::Error UserMetaData::setOriginEmailSubject(const QString &originEmailSubject)
{
    if (!originEmailSubject.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.xdg.origin.email.subject"), originEmailSubject);
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.xdg.origin.email.subject"));
    }

    return NoError;
}

QString UserMetaData::originEmailSender() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.xdg.origin.email.sender"), &value);
    return value;
}

UserMetaData::Error UserMetaData::setOriginEmailSender(const QString &originEmailSender)
{
    if (!originEmailSender.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.xdg.origin.email.sender"), originEmailSender);
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.xdg.origin.email.sender"));
    }

    return NoError;
}

QString UserMetaData::originEmailMessageId() const
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.xdg.origin.email.message-id"), &value);
    return value;
}

UserMetaData::Error UserMetaData::setOriginEmailMessageId(const QString &originEmailMessageId)
{
    if (!originEmailMessageId.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.xdg.origin.email.message-id"), originEmailMessageId);
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.xdg.origin.email.message-id"));
    }

    return NoError;
}

UserMetaData::Error UserMetaData::setAttribute(const QString& key, const QString& value)
{
    if (!value.isEmpty()) {
        k_setxattr(d->filePath, QStringLiteral("user.") + key, value);
    } else {
        k_removexattr(d->filePath, QStringLiteral("user.") + key);
    }

    return NoError;
}

bool UserMetaData::hasAttribute(const QString& key)
{
    return k_hasAttribute(d->filePath, QStringLiteral("user.") + key);
}

QString UserMetaData::attribute(const QString& key)
{
    QString value;
    k_getxattr(d->filePath, QStringLiteral("user.") + key, &value);

    return value;
}

bool UserMetaData::isSupported() const
{
    return k_isSupported(d->filePath);
}
