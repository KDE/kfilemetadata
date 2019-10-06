/*
 * This file is part of the KDE KFileMetaData project
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *               2017  Igor Poboiko <igor.poboiko@gmail.com>
 *               2019  Alexander Stippich <a.stippich@gmx.net>
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

#include <QObject>
#include <QtTest>
#include <QDebug>

#include "writercollection.h"

namespace KFileMetaData {

class WriterCollectionTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testFetchWriters()
    {
        QCoreApplication::setLibraryPaths({QCoreApplication::applicationDirPath()});
        WriterCollection collection;
        QVERIFY(collection.fetchWriters(QStringLiteral("unknown/mimetype")).isEmpty());
        QVERIFY(!collection.fetchWriters(QStringLiteral("audio/mpeg3")).isEmpty());
    }

};

}

QTEST_GUILESS_MAIN(KFileMetaData::WriterCollectionTest)

#include "writercollectiontest.moc"
