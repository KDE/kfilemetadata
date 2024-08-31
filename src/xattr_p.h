/*
    This file is part of the KDE Baloo Project
    SPDX-FileCopyrightText: 2014 Raphael Kubo da Costa <rakuco@FreeBSD.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_XATTR_P_H
#define KFILEMETADATA_XATTR_P_H

#include <QByteArray>
#include <QFile>
#include <QString>
#include <QDebug>

#if defined(Q_OS_LINUX) || defined(__GLIBC__)
#include <sys/types.h>
#include <sys/xattr.h>

#if defined(Q_OS_ANDROID) || defined(Q_OS_LINUX)
// attr/xattr.h is not available in the Android NDK so we are defining ENOATTR ourself
#ifndef ENOATTR
# define ENOATTR ENODATA        /* No such attribute */
#endif
#endif

#include <errno.h>
#elif defined(Q_OS_MAC)
#include <sys/types.h>
#include <sys/xattr.h>
#include <errno.h>
#elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
#include <sys/types.h>
#include <sys/extattr.h>
#include <errno.h>
#elif defined(Q_OS_OPENBSD)
#include <errno.h>
#elif defined(Q_OS_WIN)
#include <QFileInfo>
#include <windows.h>
#define ssize_t SSIZE_T
#endif
#include <chrono>

static KFileMetaData::UserMetaData::Attribute _mapAttribute(QByteArrayView key)
{
    using KFileMetaData::UserMetaData;
    if (key == "xdg.tags") {
        return UserMetaData::Attribute::Tags;
    }
    if (key == "baloo.rating") {
        return UserMetaData::Attribute::Rating;
    }
    if (key == "xdg.comment") {
        return UserMetaData::Attribute::Comment;
    }
    if (key == "xdg.origin.url") {
        return UserMetaData::Attribute::OriginUrl;
    }
    if (key == "xdg.origin.email.subject") {
        return UserMetaData::Attribute::OriginEmailSubject;
    }
    if (key == "xdg.origin.email.sender") {
        return UserMetaData::Attribute::OriginEmailSender;
    }
    if (key == "xdg.origin.email.message-id") {
        return UserMetaData::Attribute::OriginEmailMessageId;
    }
    return UserMetaData::Attribute::Other;
}

static KFileMetaData::UserMetaData::Attributes attributesFromEntries(const QList<QByteArray> &entries, const QByteArrayView &prefix, KFileMetaData::UserMetaData::Attributes attributes)
{
    using KFileMetaData::UserMetaData;
    UserMetaData::Attributes fileAttributes = UserMetaData::Attribute::None;
    for (const auto &entry : entries) {
        if (!entry.startsWith(prefix)) {
            continue;
        }
        fileAttributes |= _mapAttribute(QByteArrayView(entry).sliced(prefix.size()));
        fileAttributes &= attributes;
        if (fileAttributes == attributes) {
            break;
        }
    }
    return fileAttributes;
}

#if defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
inline ssize_t k_getxattr(const QString& path, const QString& name, QString* value)
{
    const QByteArray p = QFile::encodeName(path);
    const char* encodedPath = p.constData();

#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const QByteArray n = name.toUtf8();
#else
    const QByteArray n = QByteArrayView("user.") + name.toUtf8();
#endif
    const char* attributeName = n.constData();

    // First get the size of the data we are going to get to reserve the right amount of space.
#if defined(Q_OS_LINUX) || (defined(__GLIBC__) && !defined(__stub_getxattr))
    const ssize_t size = getxattr(encodedPath, attributeName, nullptr, 0);
#elif defined(Q_OS_MAC)
    const ssize_t size = getxattr(encodedPath, attributeName, NULL, 0, 0, 0);
#elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const ssize_t size = extattr_get_file(encodedPath, EXTATTR_NAMESPACE_USER, attributeName, NULL, 0);
#endif

    if (!value) {
        return size;
    }

    if (size <= 0) {
        value->clear();
        return size;
    }

    QByteArray data(size, Qt::Uninitialized);

    while (true) {
#if defined(Q_OS_LINUX) || (defined(__GLIBC__) && !defined(__stub_getxattr))
        const ssize_t r = getxattr(encodedPath, attributeName, data.data(), data.size());
#elif defined(Q_OS_MAC)
        const ssize_t r = getxattr(encodedPath, attributeName, data.data(), data.size(), 0, 0);
#elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
        const ssize_t r = extattr_get_file(encodedPath, EXTATTR_NAMESPACE_USER, attributeName, data.data(), data.size());
#endif

        if (r < 0 && errno != ERANGE) {
            value->clear();
            return r;
        }

        if (r >= 0) {
            data.resize(r);
            *value = QString::fromUtf8(data);
            return size;
        } else {
            // ERANGE
            data.resize(data.size() * 2);
        }
    }
}

inline int k_setxattr(const QString& path, const QString& name, const QString& value)
{
    const QByteArray p = QFile::encodeName(path);
    const char* encodedPath = p.constData();

#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const QByteArray n = name.toUtf8();
#else
    const QByteArray n = QByteArrayView("user.") + name.toUtf8();
#endif
    const char* attributeName = n.constData();

    const QByteArray v = value.toUtf8();
    const void* attributeValue = v.constData();

    const size_t valueSize = v.size();

#if defined(Q_OS_LINUX) || (defined(__GLIBC__) && !defined(__stub_setxattr))
    int result = setxattr(encodedPath, attributeName, attributeValue, valueSize, 0);
    return result == -1 ? errno : 0;
#elif defined(Q_OS_MAC)
    int count = setxattr(encodedPath, attributeName, attributeValue, valueSize, 0, 0);
    return count == -1 ? errno : 0;
#elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const ssize_t count = extattr_set_file(encodedPath, EXTATTR_NAMESPACE_USER, attributeName, attributeValue, valueSize);
    return count == -1 ? errno : 0;
#endif
}


inline int k_removexattr(const QString& path, const QString& name)
{
    const QByteArray p = QFile::encodeName(path);
    const char* encodedPath = p.constData();

#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const QByteArray n = name.toUtf8();
#else
    const QByteArray n = QByteArrayView("user.") + name.toUtf8();
#endif
    const char* attributeName = n.constData();

    #if defined(Q_OS_LINUX) || (defined(__GLIBC__) && !defined(__stub_removexattr))
        int result = removexattr(encodedPath, attributeName);
        return result == -1 ? errno : 0;
    #elif defined(Q_OS_MAC)
        int result = removexattr(encodedPath, attributeName, XATTR_NOFOLLOW );
        return result == -1 ? errno : 0;
    #elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
        int result = extattr_delete_file (encodedPath, EXTATTR_NAMESPACE_USER, attributeName);
        return result == -1 ? errno : 0;
    #endif
}

inline bool k_hasAttribute(const QString& path, const QString& name)
{
    auto ret = k_getxattr(path, name, nullptr);
    return (ret >= 0);
}

inline bool k_isSupported(const QString& path)
{
    auto ret = k_getxattr(path, QStringLiteral("test"), nullptr);
    return (ret >= 0) || (errno != ENOTSUP);
}

#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
static QList<QByteArray> _split_length_value(QByteArray data)
{
    int pos = 0;
    QList<QByteArray> entries;

    while (pos < data.size()) {
        unsigned char len = data[pos];
        if (pos + 1 + len <= data.size()) {
            auto value = data.mid(pos + 1, len);
            entries.append(value);
        }
        pos += 1 + len;
    }
    return entries;
}
#endif

KFileMetaData::UserMetaData::Attributes k_queryAttributes(const QString& path,
    KFileMetaData::UserMetaData::Attributes attributes)
{
    using KFileMetaData::UserMetaData;

    const QByteArray p = QFile::encodeName(path);
    const char* encodedPath = p.constData();

    #if defined(Q_OS_LINUX)
    const ssize_t size = listxattr(encodedPath, nullptr, 0);
    #elif defined(Q_OS_MAC)
    const ssize_t size = listxattr(encodedPath, nullptr, 0, 0);
    #elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const ssize_t size = extattr_list_file(encodedPath, EXTATTR_NAMESPACE_USER, nullptr, 0);
    #endif

    if (size == 0) {
        return UserMetaData::Attribute::None;
    }

    if (size < 0) {
        if (errno == E2BIG) {
            return UserMetaData::Attribute::All;
        }

        return UserMetaData::Attribute::None;
    }

    if (attributes == UserMetaData::Attribute::Any) {
        return UserMetaData::Attribute::All;
    }

    QByteArray data(size, Qt::Uninitialized);

    while (true) {
    #if defined(Q_OS_LINUX)
        const ssize_t r = listxattr(encodedPath, data.data(), data.size());
    #elif defined(Q_OS_MAC)
        const ssize_t r = listxattr(encodedPath, data.data(), data.size(), 0);
    #elif defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
        const ssize_t r = extattr_list_file(encodedPath, EXTATTR_NAMESPACE_USER, data.data(), data.size());
    #endif

        if (r == 0) {
            return UserMetaData::Attribute::None;
        }

        if (r < 0 && errno != ERANGE) {
            return UserMetaData::Attribute::None;
        }

        if (r > 0) {
            data.resize(r);
            break;
        } else {
            data.resize(data.size() * 2);
        }
    }

#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD)
    const QByteArrayView prefix;
    const auto entries = _split_length_value(data);
#else
    const QByteArrayView prefix("user.");
    const auto entries = data.split('\0');
#endif

    return attributesFromEntries(entries, prefix, attributes);
}

#elif  defined(Q_OS_WIN)

inline ssize_t k_getxattr(const QString& path, const QString& name, QString* value)
{
    const QString fullADSName = path +
        QLatin1String(":user.") + name;
    HANDLE hFile = ::CreateFileW(reinterpret_cast<const WCHAR*>(fullADSName.utf16()), GENERIC_READ, FILE_SHARE_READ, NULL,
             OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = ::GetLastError();
        std::string message = std::system_category().message(error);
        qWarning() << "failed to open ADS:" << message << fullADSName;
        return -1;
    }

    LARGE_INTEGER lsize;
    BOOL ret = GetFileSizeEx(hFile, &lsize);

    if (!ret || lsize.QuadPart > 0x7fffffff || lsize.QuadPart == 0) {
        CloseHandle(hFile);
        value->clear();
        return lsize.QuadPart == 0 ? 0 : -1;
    }

    DWORD r = 0;
    QByteArray data(lsize.QuadPart, Qt::Uninitialized);
    // should we care about attributes longer than 2GiB? - unix xattr are restricted to much lower values
    ret = ::ReadFile(hFile, data.data(), data.size(), &r, NULL);
    CloseHandle(hFile);

    if (!ret) {
        DWORD error = ::GetLastError();
        std::string message = std::system_category().message(error);
        qWarning() << "failed to open ADS:" << message << fullADSName;
        return -1;
    }

    if (r == 0) {
        value->clear();
        return 0;
    }

    data.resize(r);

    *value = QString::fromUtf8(data);
    return r;
}

inline int k_setxattr(const QString& path, const QString& name, const QString& value)
{
    const QString fullADSName = path + QLatin1String(":user.") + name;
    if (fullADSName.size() > MAX_PATH) {
        // https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file#maximum-path-length-limitation
        // We could handle longer file names but it would require special casing per-windows version
        return ERROR_FILENAME_EXCED_RANGE;
    }

    HANDLE hFile = ::CreateFileW(reinterpret_cast<const WCHAR*>(fullADSName.utf16()), GENERIC_WRITE, 0, NULL,
             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = ::GetLastError();
        std::string message = std::system_category().message(error);
        qWarning() << "failed to open file to write to ADS:" << message << error << fullADSName;
        return -1; // unknown error
    }

    DWORD count = 0;

    const QByteArray v = value.toUtf8();
    if(!::WriteFile(hFile, v.constData(), v.size(), &count, NULL)) {
        DWORD error = ::GetLastError();
        std::string message = std::system_category().message(error);
        qWarning() << "failed to write to ADS:" << message << error << fullADSName;

        CloseHandle(hFile);
        return -1; // unknown error
    }

    CloseHandle(hFile);
    return 0; // Success
}

inline bool k_hasAttribute(const QString& path, const QString& name)
{
    QString fullpath = path + QLatin1String(":user.") + name;
    HANDLE hFile = CreateFileW(reinterpret_cast<const WCHAR*>(fullpath.utf16()), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        // ERROR_FILE_NOT_FOUND supposedly
        return false;
    }
    // ADS exists
    CloseHandle(hFile);
    return true;
}

inline int k_removexattr(const QString& path, const QString& name)
{
    const QString fullADSName = path + QLatin1String(":user.") + name;
    int ret = (DeleteFileW(reinterpret_cast<const WCHAR*>(fullADSName.utf16()))) ? 0 : -1;
    return ret;
}

inline bool k_isSupported(const QString& path)
{
    QFileInfo f(path);
    const QString drive = QString(f.absolutePath().left(2)) + QStringLiteral("\\");
    WCHAR szFSName[MAX_PATH];
    DWORD dwVolFlags;
    ::GetVolumeInformationW(reinterpret_cast<const WCHAR*>(drive.utf16()), NULL, 0, NULL, NULL, &dwVolFlags, szFSName, MAX_PATH);
    return ((dwVolFlags & FILE_NAMED_STREAMS) && _wcsicmp(szFSName, L"NTFS") == 0);
}

KFileMetaData::UserMetaData::Attributes k_queryAttributes(const QString& path,
    KFileMetaData::UserMetaData::Attributes attributes)
{
    using KFileMetaData::UserMetaData;

    if (!k_isSupported(path)) {
        return UserMetaData::Attribute::None;
    }

    HANDLE hFile= ::CreateFile(reinterpret_cast<const WCHAR*>(path.utf16()), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE , NULL,
                                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        DWORD error = ::GetLastError();
        std::string message = std::system_category().message(error);
        qWarning() << "failed CreateFile:" << message << path << error;
        return UserMetaData::Attribute::None;
    }

    QList<QByteArray> entries;
    QString entry;
    FILE_STREAM_INFO* fi = new FILE_STREAM_INFO[256];
    if (::GetFileInformationByHandleEx(hFile, FileStreamInfo, fi, 256 * sizeof(FILE_STREAM_INFO))) {
        // ignore first entry it is "::$DATA"
        FILE_STREAM_INFO* p = fi;
        while (p->NextEntryOffset != NULL) {
            p = (FILE_STREAM_INFO*) ((char*)p + p->NextEntryOffset);
            entry = QString::fromUtf16((char16_t*)p->StreamName, p->StreamNameLength / sizeof(char16_t));
            // entries are of the form ":user.key:$DATA"
            entry.chop(6);
            entries.append(entry.sliced(1).toLocal8Bit());
        }
    } else {
        DWORD error = ::GetLastError();
        if (error != ERROR_HANDLE_EOF) {
            std::string message = std::system_category().message(error);
            qWarning() << "failed GetFileInformationByHandleEx:" << message << path << hFile;
        }
    }
    delete[] fi;
    CloseHandle(hFile);

    if (entries.size() == 0) {
        return UserMetaData::Attribute::None;
    }

    if (entries.size() > 0 && attributes == UserMetaData::Attribute::Any) {
        return UserMetaData::Attribute::All;
    }

    const QByteArrayView prefix("user.");
    return attributesFromEntries(entries, prefix, attributes);
}

#else
inline ssize_t k_getxattr(const QString&, const QString&, QString*)
{
    return 0;
}

inline int k_setxattr(const QString&, const QString&, const QString&)
{
    return -1;
}

inline int k_removexattr(const QString&, const QString&)
{
    return -1;
}

inline bool k_hasAttribute(const QString&, const QString&)
{
    return false;
}

inline bool k_isSupported(const QString&)
{
    return false;
}

KFileMetaData::UserMetaData::Attributes k_queryAttributes(const QString&,
    KFileMetaData::UserMetaData::Attributes attributes)
{
    return KFileMetaData::UserMetaData::Attribute::None;
}

#endif

#endif // KFILEMETADATA_XATTR_P_H
