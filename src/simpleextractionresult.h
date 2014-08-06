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

#ifndef KFILEMETADATA_SimpleExtractionResult_H
#define KFILEMETADATA_SimpleExtractionResult_H

#include "extractionresult.h"
#include <QVector>
#include <QString>

namespace KFileMetaData {

/**
 * A simple ExtractionResult implementation which stores
 * all the data in memory.
 *
 * This should ideally not be used in production applications as
 * it holds all of the plain text in memory, and that can get quite large
 * when extracting certain documents.
 */
class KFILEMETADATA_EXPORT SimpleExtractionResult : public ExtractionResult
{
public:
    SimpleExtractionResult(const QString& url, const QString& mimetype, const Flags& flags = ExtractEverything);
    SimpleExtractionResult(const SimpleExtractionResult& rhs);
    virtual ~SimpleExtractionResult();

    SimpleExtractionResult& operator=(const SimpleExtractionResult& rhs);
    bool operator==(const SimpleExtractionResult& rhs) const;

    virtual void add(Property::Property property, const QVariant& value);
    virtual void addType(Type::Type type);
    virtual void append(const QString& text);

    PropertyMap properties() const;
    QString text() const;
    QVector<Type::Type> types() const;

private:
    class Private;
    Private* d;
};

}

#endif // KFILEMETADATA_SimpleExtractionResult_H
