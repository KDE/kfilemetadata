/*
    EmbeddedImageData tests.

    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef EMBEDDEDIMAGEDATATEST_H
#define EMBEDDEDIMAGEDATATEST_H

#include <QObject>
#include "properties.h"

class EmbeddedImageDataTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;
    QByteArray m_coverImage;

private Q_SLOTS:
    void initTestCase();
    void test();
    void test_data();
    void testWrite();
    void testWrite_data();
    void testDelete();
    void testDelete_data();
    void testDeleteInsert();
    void testDeleteInsert_data();
    void testMultiImage();
    void testMultiImage_data();
};

#endif // EMBEDDEDIMAGEDATATEST_H
