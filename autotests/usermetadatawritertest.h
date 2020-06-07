/*
    SPDX-FileCopyrightText: 2017 James D. Smith <smithjd15@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef USERMETADATAWRITERTEST_H
#define USERMETADATAWRITERTEST_H

#include <QObject>

class UserMetaDataWriterTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void initTestCase();
    void test();
    void testDanglingSymlink();
    void cleanupTestCase();
};

#endif // USERMETADATAWRITERTEST_H
