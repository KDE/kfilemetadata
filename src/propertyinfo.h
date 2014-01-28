/*
 * This file is part of the KFileMetaData project
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _KFILEMETADATA_PROPERTYINFO_H
#define _KFILEMETADATA_PROPERTYINFO_H

#include <QString>
#include <QVariant>
#include "properties.h"
#include "kfilemetadata_export.h"

namespace KFileMetaData {

class KFILEMETADATA_EXPORT PropertyInfo
{
public:
    PropertyInfo(Property::Property property);
    PropertyInfo(const PropertyInfo& pi);
    ~PropertyInfo();

    /**
     * The enumeration which represents this property
     */
    Property::Property property() const;

    /**
     * The internal developer named used to refer to the property
     */
    QString name() const;

    /**
     * A user visible name of the property
     */
    QString displayName() const;

    /**
     * The type the value of this property should be.
     * Eg - Property::Height should be an integer
     */
    QVariant::Type valueType() const;

    /**
     * Indicates if this property requires indexing or should just be stored.
     * Eg - Property::Height does not need to be part of the global index.
     *      When a user searches for 600, they should not get images with
     *      that height
     */
    bool shouldBeIndexed() const;

    /**
     * Construct a PropertyInfo from the internal property name.
     * The internal property name is case insensitive
     */
    static PropertyInfo fromName(const QString& name);

private:
    class Private;
    Private* d;
};

}
#endif // _KFILEMETADATA_PROPERTYINFO_H
