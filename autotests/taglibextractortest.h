/*
 * TagLibExtractor tests.
 *
 * SPDX-FileCopyrightText: 2015 Juan Palacios <jpalaciosdev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
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
    void initTestCase();
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
    void testAax();
    void testAax_data();
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
    void testImageData();
    void testImageData_data();

private:
    // Convenience function
    const QStringList propertyEnumNames(const QList<KFileMetaData::Property::Property>& key) const;
    QMimeDatabase mimeDb;
    QByteArray m_coverImage;
};

#endif // TAGLIBEXTRACTORTEST_H
