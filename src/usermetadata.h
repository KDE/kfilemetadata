/*
 * <one line to give the library's name and an idea of what it does.>
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

#ifndef KFILEMETADATA_USERMETADATA_H
#define KFILEMETADATA_USERMETADATA_H

#include "kfilemetadata_export.h"
#include <QStringList>
#include <QUrl>

namespace KFileMetaData {

/**
 * \class UserMetaData usermetadata.h <KFileMetaData/UserMetaData>
 */
class KFILEMETADATA_EXPORT UserMetaData {
public:
    UserMetaData(const QString &filePath);
    UserMetaData(const UserMetaData &rhs);
    virtual ~UserMetaData();

    enum Error {
        NoError = 0
    };
    const UserMetaData& operator =(const UserMetaData& rhs);

    QString filePath() const;
    bool isSupported() const;

    Error setTags(const QStringList& tags);
    QStringList tags() const;

    int rating() const;
    Error setRating(int rating);

    QString userComment() const;
    Error setUserComment(const QString& userComment);

    QUrl originUrl() const;
    Error setOriginUrl(const QUrl &originUrl);

    QString originEmailSubject() const;
    Error setOriginEmailSubject(const QString &originEmailSubject);

    QString originEmailSender() const;
    Error setOriginEmailSender(const QString &originEmailSender);

    QString originEmailMessageId() const;
    Error setOriginEmailMessageId(const QString &originEmailMessageId);

    QString attribute(const QString& name);
    Error setAttribute(const QString& name, const QString& value);
    bool hasAttribute(const QString& name);
private:
    class Private;
    Private *d;
};

}

#endif // KFILEMETADATA_USERMETADATA_H
