/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_TYPEINFO_H
#define KFILEMETADATA_TYPEINFO_H

#include "types.h"
#include "kfilemetadata_export.h"
#include <QString>
#include <memory>

namespace KFileMetaData {

class TypeInfoPrivate;
/**
 * \class TypeInfo typeinfo.h <KFileMetaData/TypeInfo>
 */
class KFILEMETADATA_EXPORT TypeInfo
{
public:
    TypeInfo(Type::Type type);
    TypeInfo(const TypeInfo& ti);
    ~TypeInfo();

    TypeInfo& operator=(const TypeInfo& rhs);

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(5, 91)
    /*
     * @deprecated since 5.91 Use TypeInfo::operator==() const instead.
     */
KFILEMETADATA_DEPRECATED_VERSION(5, 91, "Use TypeInfo::operator==() const instead")
    bool operator==(const TypeInfo& rhs);
#endif
    /*
     * @since 5.91
     */
    bool operator==(const TypeInfo& rhs) const;

    /**
     * The type identifier
     */
    Type::Type type() const;

    /**
     * An internal unique name for the type
     */
    QString name() const;

    /**
     * A user visible translated name for this type
     */
    QString displayName() const;

    /**
     * Construct a TypeInfo from the internal type name.
     * The internal type name is case insensitive
     */
    static TypeInfo fromName(const QString& name);

private:
    const std::unique_ptr<TypeInfoPrivate> d;
};
}

#endif // KFILEMETADATA_TYPEINFO_H
