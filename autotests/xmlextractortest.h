/*
    SPDX-FileCopyrightText: 2018 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#ifndef XMLEXTRACTORTESTS_H
#define XMLEXTRACTORTESTS_H

#include <QObject>
#include <QString>

class XmlExtractorTests : public QObject
{
    Q_OBJECT
public:
    explicit XmlExtractorTests(QObject* parent = nullptr);

private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testNoExtraction();
    void benchMarkXmlExtractor();
    void testXmlExtractor();
    void testXmlExtractorNoContent();
    void testXmlExtractorNoContentDcterms();
    void testXmlExtractorContainer();
    void testXmlExtractorMathML();
};

#endif // XMLEXTRACTORTESTS_H
