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

#include "simpleresult.h"

using namespace KFileMetaData;

SimpleResult::SimpleResult(const QString& url, const QString& mimetype)
    : ExtractionResult(url, mimetype)
{
}

void SimpleResult::add(Property::Property property, const QVariant& value)
{
    m_properties.insertMulti(property, value);
}

void SimpleResult::addType(Type::Type type)
{
    m_types << type;
}

void SimpleResult::append(const QString& text)
{
    m_text.append(text);
    m_text.append(QLatin1Char(' '));
}

PropertyMap SimpleResult::properties() const
{
    return m_properties;
}

QString SimpleResult::text() const
{
    return m_text;
}

QVector<Type::Type> SimpleResult::types() const
{
    return m_types;
}
