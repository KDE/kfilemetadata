/*
    This file is part of the Nepomuk KDE project.
    SPDX-FileCopyrightText: 2013 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef INDEXEREXTRACTORTESTS_H
#define INDEXEREXTRACTORTESTS_H

#include <QObject>
#include <QString>

class IndexerExtractorTests : public QObject
{
    Q_OBJECT
public:
    explicit IndexerExtractorTests(QObject* parent = nullptr);

private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testNoExtraction();
    void benchMarkPlainTextExtractor();
    void testPlainTextExtractor();
    void testPlainTextExtractorNoPlainText();
};

#endif // INDEXERTESTS_H
