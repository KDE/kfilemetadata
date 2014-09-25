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

namespace KFileMetaData {

class KFILEMETADATA_EXPORT UserMetaData {
public:
    UserMetaData(const QString &filePath);
    UserMetaData(const UserMetaData &rhs);
    virtual ~UserMetaData();

    const UserMetaData& operator =(const UserMetaData& rhs);

    QString filePath() const;
    bool isSupported() const;

    void setTags(const QStringList& tags);
    void addTags(const QStringList& tags);
    void addTag(const QString& tag);
    void removeTags(const QStringList& tags);
    void removeTag(const QString& tag);

    QStringList tags() const;

    int rating() const;
    void setRating(int rating);

    QString userComment() const;
    void setUserComment(const QString& userComment);

    QString custom(const QString& key);
    void setCustom(const QString& key, const QString& value);
    bool hasCustom(const QString& key);

    // FIXME: Error handling?
private:
    class Private;
    Private *d;
};

}

#endif // KFILEMETADATA_USERMETADATA_H
