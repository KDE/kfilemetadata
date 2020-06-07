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

private Q_SLOTS:
    void test();
    void test_data();
    void testWrite();
    void testWrite_data();
};

#endif // EMBEDDEDIMAGEDATATEST_H
