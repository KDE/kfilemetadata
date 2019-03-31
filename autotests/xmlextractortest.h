/*
   Copyright (C) 2018  Stefan Brüns <stefan.bruens@rwth-aachen.de>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
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
    void benchMarkXmlExtractor();
    void testXmlExtractor();
    void testXmlExtractorNoContent();
    void testXmlExtractorContainer();
    void testXmlExtractorMathML();
};

#endif // XMLEXTRACTORTESTS_H
