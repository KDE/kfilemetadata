/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef OFFICE2007EXTRACTORTEST_H
#define OFFICE2007EXTRACTORTEST_H

#include <QObject>

class Office2007ExtractorTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testNoExtraction();
    void test();
    void testMetaDataOnly();
};

#endif // OFFICE2007EXTRACTORTEST_H
