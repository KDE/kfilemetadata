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

/*!
 * \class KFileMetaData::UserMetaData
 * \inheaderfile KFileMetaData/UserMetaData
 * \inmodule KFileMetaData
 *
 * \brief The UserMetaData class can be used to read and set user meta data of files.
 */
class KFILEMETADATA_EXPORT UserMetaData
{
public:
    /*!
     *
     */
    UserMetaData(const QString &filePath);
    UserMetaData(const UserMetaData &rhs);
    virtual ~UserMetaData();

    /*!
     * \value NoError i.e. Success
     * \value[since 6.2] UnknownError An error that's not currently handled specifically
     * \value[since 6.2] NotSupported Underlying filesystem does not provide extended attributes features
     * \value[since 6.2] NoSpace There is insufficient space remaining to store the extended attribute
     * \value[since 6.2] MissingPermission Process doesn't have write permission to the file or the file is marked append-only
     * \value[since 6.2] ValueTooBig The value size exceeds the maximum size allowed per-value (64 kB for Linux VFS
     * \value[since 6.2] NameToolong The attribute name is too long (255 bytes for Linux VFS)
     *
     */
    enum Error {
        NoError = 0,
        UnknownError,
        NotSupported,
        NoSpace,
        MissingPermission,
        ValueTooBig,
        NameToolong,
    };

    /*!
     * \value None
     * \value Any
     * \value Tags
     * \value Rating
     * \value Comment
     * \value OriginUrl
     * \value OriginEmailSubject
     * \value OriginEmailSender
     * \value OriginEmailMessageId
     * \value Other
     * \value All
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
    Q_DECLARE_FLAGS(Attributes, Attribute)

    const UserMetaData& operator =(const UserMetaData& rhs);

    /*!
     *
     */
    QString filePath() const;

    /*!
     *
     */
    bool isSupported() const;

    /*!
     *
     */
    Error setTags(const QStringList& tags);

    /*!
     *
     */
    QStringList tags() const;

    /*!
     *
     */
    int rating() const;

    /*!
     *
     */
    Error setRating(int rating);

    /*!
     *
     */
    QString userComment() const;

    /*!
     *
     */
    Error setUserComment(const QString& userComment);

    /*!
     *
     */
    QUrl originUrl() const;

    /*!
     *
     */
    Error setOriginUrl(const QUrl &originUrl);

    /*!
     *
     */
    QString originEmailSubject() const;

    /*!
     *
     */
    Error setOriginEmailSubject(const QString &originEmailSubject);

    /*!
     *
     */
    QString originEmailSender() const;

    /*!
     *
     */
    Error setOriginEmailSender(const QString &originEmailSender);

    /*!
     *
     */
    QString originEmailMessageId() const;

    /*!
     *
     */
    Error setOriginEmailMessageId(const QString &originEmailMessageId);

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(6, 2)
    /*!
     * \deprecated[6.2]
     */
    QString attribute(const QString &name);
#endif
    /*!
     * \since 6.2
     */
    QString attribute(const QString &name) const;

    /*!
     *
     */
    Error setAttribute(const QString &name, const QString &value);

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(6, 2)
    /*!
     * \deprecated[6.2]
     */
    bool hasAttribute(const QString &name);
#endif
    /*!
     * \since 6.2
     */
    bool hasAttribute(const QString &name) const;

    /*!
      * Query list of available attributes
      *
      * Checks for the availability of the given \a attributes. May return
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
