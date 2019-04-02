/*
 * Copyright (C) 2016 Varun Joshi <varunj.1011@gmail.com>
 * Copyright (C) 2018 Alexander Stippich <a.stippich@gmx.net>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef TAGLIBWRITERTEST_H
#define TAGLIBWRITERTEST_H

#include <QObject>

class TagLibWriterTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testCommonData();
    void testCommonData_data();
    void testExtendedData();
    void testExtendedData_data();
    void testRating();
    void testRating_data();
};

#endif // TAGLIBWRITERTEST_H
