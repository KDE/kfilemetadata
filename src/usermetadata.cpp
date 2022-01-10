/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "usermetadata.h"
#include "xattr_p.h"


using namespace KFileMetaData;

class KFileMetaData::UserMetaDataPrivate
{
public:
    QString filePath;
};

UserMetaData::UserMetaData(const QString& filePath)
    : d(new UserMetaDataPrivate)
{
    d->filePath = filePath;
}

UserMetaData::UserMetaData(const UserMetaData& rhs)
    : d(new UserMetaDataPrivate(*rhs.d))
{
}

UserMetaData::~UserMetaData() = default;

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
    return value.split(QLatin1Char(','), Qt::SkipEmptyParts);
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

UserMetaData::Attributes UserMetaData::queryAttributes(UserMetaData::Attributes attributes) const
{
    return k_queryAttributes(d->filePath, attributes);
}
