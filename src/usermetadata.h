/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_USERMETADATA_H
#define KFILEMETADATA_USERMETADATA_H

#include "kfilemetadata_export.h"
#include <QStringList>
#include <QUrl>

#include <memory>

namespace KFileMetaData {

class UserMetaDataPrivate;
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

    /**
     * @see Attributes
     */
    enum Attribute : uint32_t {
        None                 = 0x0,
        Any                  = None,
        Tags                 = 0x1,
        Rating               = 0x2,
        Comment              = 0x4,
        OriginUrl            = 0x8,
        OriginEmailSubject   = 0x10,
        OriginEmailSender    = 0x20,
        OriginEmailMessageId = 0x40,
        Other                = 0xffffff80,
        All                  = 0xffffffff,
    };
    /**
     * Stores a combination of #Attribute values.
     */
    Q_DECLARE_FLAGS(Attributes, Attribute)

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

    /**
      * Query list of available attributes
      *
      * Checks for the availability of the given \p attributes. May return
      * a superset of the input value when the file has attributes set
      * beyond the requested ones.
      *
      * If the input attribute mask is Attribute::Any, either Attribute::None
      * (the file has no user attributes) or Attribute::All (the file has at
      * least one attribute set) is returned.
      *
      * \since 5.60
      */
    Attributes queryAttributes(Attributes attributes = Attribute::Any) const;

private:
    const std::unique_ptr<UserMetaDataPrivate> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(UserMetaData::Attributes)
}

#endif // KFILEMETADATA_USERMETADATA_H
