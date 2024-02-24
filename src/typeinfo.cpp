/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "typeinfo.h"

#include <KLazyLocalizedString>
#include <KLocalizedString>

#include "icnamematch_p.h"

#include <array>

using namespace KFileMetaData;

class KFileMetaData::TypeInfoPrivate
{
public:
    const Type::Type type;
    const QString name;
    const KLazyLocalizedString displayName;
};

const static TypeInfoPrivate staticEmptyTypeInfo{ Type::Empty, QStringLiteral("empty"), kli18nc("@label", "Empty") };

const static std::array<TypeInfoPrivate, 9> staticTypeInfo
{
    TypeInfoPrivate{ Type::Archive,      QStringLiteral("Archive"),      kli18nc("@label", "Archive") },
    TypeInfoPrivate{ Type::Audio,        QStringLiteral("Audio"),        kli18nc("@label", "Audio") },
    TypeInfoPrivate{ Type::Document,     QStringLiteral("Document"),     kli18nc("@label", "Document") },
    TypeInfoPrivate{ Type::Image,        QStringLiteral("Image"),        kli18nc("@label", "Image") },
    TypeInfoPrivate{ Type::Presentation, QStringLiteral("Presentation"), kli18nc("@label", "Presentation") },
    TypeInfoPrivate{ Type::Spreadsheet,  QStringLiteral("Spreadsheet"),  kli18nc("@label", "Spreadsheet") },
    TypeInfoPrivate{ Type::Text,         QStringLiteral("Text"),         kli18nc("@label", "Text") },
    TypeInfoPrivate{ Type::Video,        QStringLiteral("Video"),        kli18nc("@label", "Video") },
    TypeInfoPrivate{ Type::Folder,       QStringLiteral("Folder"),       kli18nc("@label", "Folder") },
};

namespace {
     auto constexpr typeDataById(Type::Type type) {
         for (const auto& t : staticTypeInfo) {
             if (t.type == type)
                 return &t;
         }
         return &staticEmptyTypeInfo;
     }
}

TypeInfo::TypeInfo()
    : d(&staticEmptyTypeInfo) {};

TypeInfo::TypeInfo(Type::Type type)
    : d(typeDataById(type))
{
}

TypeInfo::TypeInfo(const TypeInfo& ti)
    : d(ti.d)
{
}

TypeInfo::~TypeInfo() = default;

TypeInfo& TypeInfo::operator=(const TypeInfo& rhs)
{
    d = rhs.d;
    return *this;
}

bool TypeInfo::operator==(const TypeInfo& rhs) const
{
    return d == rhs.d;
}

QString TypeInfo::displayName() const
{
    return d->displayName.toString();
}

QString TypeInfo::name() const
{
    return d->name;
}

Type::Type TypeInfo::type() const
{
    return d->type;
}

namespace {
    static const QHash<LcIdentifierName, TypeInfo> typeHash = []() {
        QHash<LcIdentifierName, TypeInfo> infoHash;
        infoHash.reserve(staticTypeInfo.size());

        for (const auto& info: staticTypeInfo) {
            infoHash[info.name] = info.type;
        }
        return infoHash;
    }();
}

TypeInfo TypeInfo::fromName(const QString& name)
{
    return typeHash.value(LcIdentifierName(name));
}

QStringList TypeInfo::allNames()
{
    static QStringList sNames = []() {
        QStringList names;
        names.reserve(staticTypeInfo.size());
        for (auto info: staticTypeInfo) {
            names.append(info.name);
        }
        return names;
    }();
    return sNames;
}
