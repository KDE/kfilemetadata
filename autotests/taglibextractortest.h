/*
 * TagLibExtractor tests.
 *
 * Copyright (C) 2015  Juan Palacios <jpalaciosdev@gmail.com>
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

#ifndef TAGLIBEXTRACTORTEST_H
#define TAGLIBEXTRACTORTEST_H

#include <QObject>
#include <QMimeDatabase>
#include "properties.h"

class TagLibExtractorTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testNoExtraction();
    void testPropertyTypes();
    void testCommonData();
    void testCommonData_data();
    void testVorbisComment();
    void testVorbisComment_data();
    void testVorbisCommentMultivalue();
    void testVorbisCommentMultivalue_data();
    void testId3();
    void testId3_data();
    void testApe();
    void testApe_data();
    void testMp4();
    void testMp4_data();
    void testAsf();
    void testAsf_data();
    void testId3Rating_data();
    void testId3Rating();
    void testWmaRating_data();
    void testWmaRating();
    void testNoMetadata();
    void testNoMetadata_data();
    void testRobustness();
    void testRobustness_data();

private:
    // Convenience function
    const QStringList propertyEnumNames(const QList<KFileMetaData::Property::Property>& key) const;
    QMimeDatabase mimeDb;
};

#endif // TAGLIBEXTRACTORTEST_H
