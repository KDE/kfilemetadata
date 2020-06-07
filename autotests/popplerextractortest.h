/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef POPPLEREXTRACTORTEST_H
#define POPPLEREXTRACTORTEST_H

#include <QObject>

class PopplerExtractorTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testNoExtraction();
    void test();
    void testMetaDataOnly();
};

#endif // POPPLEREXTRACTORTEST_H
