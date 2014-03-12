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

#ifndef KFILEMETADATA_SIMPLERESULT_H
#define KFILEMETADATA_SIMPLERESULT_H

#include "extractionresult.h"
#include <QVector>
#include <QString>

namespace KFileMetaData {

class SimpleResult : public ExtractionResult
{
public:
    SimpleResult(const QString& url, const QString& mimetype);

    virtual void add(Property::Property property, const QVariant& value);
    virtual void addType(Type::Type type);
    virtual void append(const QString& text);

    PropertyMap properties() const;
    QString text() const;
    QVector<Type::Type> types() const;

private:
    PropertyMap m_properties;
    QString m_text;
    QVector<Type::Type> m_types;
};

}

#endif // KFILEMETADATA_SIMPLERESULT_H
