/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KFILEMETADATA_SimpleExtractionResult_H
#define KFILEMETADATA_SimpleExtractionResult_H

#include "extractionresult.h"
#include <QVector>
#include <QString>

#include <memory>

namespace KFileMetaData {
enum PropertiesMapType { MultiMap };

class SimpleExtractionResultPrivate;

/**
 * \class SimpleExtractionResult simpleextractionresult.h <KFileMetaData/SimpleExtractionResult>
 *
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
    SimpleExtractionResult(const QString& url, const QString& mimetype = QString(), const Flags& flags = Flags{ExtractPlainText | ExtractMetaData});
    SimpleExtractionResult(const SimpleExtractionResult& rhs);
    ~SimpleExtractionResult() override;

    SimpleExtractionResult& operator=(const SimpleExtractionResult& rhs);
    bool operator==(const SimpleExtractionResult& rhs) const;

    void add(Property::Property property, const QVariant& value) override;
    void addType(Type::Type type) override;
    void append(const QString& text) override;

#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(5, 89)
    /// @deprecated Since 5.89, use properties(PropertiesMapType) overload instead
    KFILEMETADATA_DEPRECATED_VERSION(5, 89, "Use properties(PropertiesMapType) overload instead")
    PropertyMap properties() const;
#endif

    /**
     * Returns the properties of the extraction result.
     * Because QMap::insertMulti is deprecated, this overload returns a QMultiMap.
     * To automaticalls use the overload with the default value, define the KFILEMETADATA_DISABLE_DEPRECATED_BEFORE_AND_AT
     * in the cmake code to the deprecating version of this method or greater.
     * For example:
     * @code
     * add_definitions(-DKFILEMETADATA_DISABLE_DEPRECATED_BEFORE_AND_AT=0x055900)
     * @endcode
     * @since 5.89
     */
#if KFILEMETADATA_ENABLE_DEPRECATED_SINCE(5, 89)
    PropertyMultiMap properties(PropertiesMapType) const;
#else
    PropertyMultiMap properties(PropertiesMapType = PropertiesMapType::MultiMap) const;
#endif

    QString text() const;
    QVector<Type::Type> types() const;

private:
    const std::unique_ptr<SimpleExtractionResultPrivate> d;
};

}

#endif // KFILEMETADATA_SimpleExtractionResult_H
