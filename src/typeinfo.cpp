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

#include "typeinfo.h"

#include <KLocalizedString>

using namespace KFileMetaData;

class TypeInfo::Private {
public:
    Type::Type type;
    QString name;
    QString displayName;
};

TypeInfo::TypeInfo(Type::Type type)
    : d(new Private)
{
    d->type = type;

    switch (type) {
    case Type::Empty:
        d->name = QStringLiteral("empty");
        d->displayName = QString();
        break;

    case Type::Archive:
        d->name = QStringLiteral("Archive");
        d->displayName = i18nc("@label", "Archive");
        break;

    case Type::Audio:
        d->name = QStringLiteral("Audio");
        d->displayName = i18nc("@label", "Audio");
        break;

    case Type::Document:
        d->name = QStringLiteral("Document");
        d->displayName = i18nc("@label", "Document");
        break;

    case Type::Image:
        d->name = QStringLiteral("Image");
        d->displayName = i18nc("@label", "Image");
        break;

    case Type::Presentation:
        d->name = QStringLiteral("Presentation");
        d->displayName = i18nc("@label", "Presentation");
        break;

    case Type::Spreadsheet:
        d->name = QStringLiteral("Spreadsheet");
        d->displayName = i18nc("@label", "Spreadsheet");
        break;

    case Type::Text:
        d->name = QStringLiteral("Text");
        d->displayName = i18nc("@label", "Text");
        break;

    case Type::Video:
        d->name = QStringLiteral("Video");
        d->displayName = i18nc("@label", "Video");
        break;

    case Type::Folder:
        d->name = QStringLiteral("Folder");
        d->displayName = i18nc("@label", "Folder");
        break;
    }
}

TypeInfo::TypeInfo(const TypeInfo& ti)
    : d(new Private(*ti.d))
{
}

TypeInfo::~TypeInfo()
{
    delete d;
}

TypeInfo& TypeInfo::operator=(const TypeInfo& rhs)
{
    *d = *rhs.d;
    return *this;
}

bool TypeInfo::operator==(const TypeInfo& rhs)
{
    return d->type == rhs.d->type && d->name == rhs.d->name && d->displayName == rhs.d->displayName;
}

QString TypeInfo::displayName() const
{
    return d->displayName;
}

QString TypeInfo::name() const
{
    return d->name;
}

Type::Type TypeInfo::type() const
{
    return d->type;
}

TypeInfo TypeInfo::fromName(const QString& name)
{
    for (int t = static_cast<int>(Type::FirstType); t <= static_cast<int>(Type::LastType); t++) {
        TypeInfo ti(static_cast<Type::Type>(t));
        if (ti.name().compare(name, Qt::CaseInsensitive) == 0) {
            return ti;
        }
    }

    return TypeInfo(Type::Empty);
}
