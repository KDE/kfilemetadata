/*
    SPDX-FileCopyrightText: 2016 Varun Joshi <varunj.1011@gmail.com>
    SPDX-FileCopyrightText: 2018 Alexander Stippich <a.stippich@gmx.net>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef TAGLIBWRITERTEST_H
#define TAGLIBWRITERTEST_H

#include <QObject>

namespace KFileMetaData {
    class ExtractionResult;
}

class TagLibWriterTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;
    void extractResult(const QString &mimeType, KFileMetaData::ExtractionResult &result);

private Q_SLOTS:
    void testCommonData();
    void testCommonData_data();
    void testExtendedData();
    void testExtendedData_data();
    void testRating();
    void testRating_data();
    void testComplexContactData();
    void testComplexContactData_data();
};

#endif // TAGLIBWRITERTEST_H
