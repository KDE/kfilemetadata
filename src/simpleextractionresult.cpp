/*
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

#include "simpleextractionresult.h"

using namespace KFileMetaData;

class Q_DECL_HIDDEN SimpleExtractionResult::Private
{
public:
    PropertyMap m_properties;
    QString m_text;
    QVector<Type::Type> m_types;
};

SimpleExtractionResult::SimpleExtractionResult(const QString& url, const QString& mimetype, const Flags& flags)
    : ExtractionResult(url, mimetype, flags)
    , d(new Private)
{
}

SimpleExtractionResult::SimpleExtractionResult(const SimpleExtractionResult& rhs)
    : ExtractionResult(*this)
    , d(new Private(*rhs.d))
{
}

SimpleExtractionResult::~SimpleExtractionResult()
{
    delete d;
}

SimpleExtractionResult& SimpleExtractionResult::operator=(const SimpleExtractionResult& rhs)
{
    *d = *rhs.d;
    return *this;
}

bool SimpleExtractionResult::operator==(const SimpleExtractionResult& rhs) const
{
    return d->m_properties == rhs.d->m_properties && d->m_text == rhs.d->m_text
           && d->m_types == rhs.d->m_types;
}

void SimpleExtractionResult::add(Property::Property property, const QVariant& value)
{
    d->m_properties.insertMulti(property, value);
}

void SimpleExtractionResult::addType(Type::Type type)
{
    d->m_types << type;
}

void SimpleExtractionResult::append(const QString& text)
{
    d->m_text.append(text);
    d->m_text.append(QLatin1Char(' '));
}

PropertyMap SimpleExtractionResult::properties() const
{
    return d->m_properties;
}

QString SimpleExtractionResult::text() const
{
    return d->m_text;
}

QVector<Type::Type> SimpleExtractionResult::types() const
{
    return d->m_types;
}
