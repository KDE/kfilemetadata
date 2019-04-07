/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 * Copyright (C) 2016  Christoph Cullmann <cullmann@kde.org>
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

#ifndef ODFEXTRACTORTEST_H
#define ODFEXTRACTORTEST_H

#include <QObject>
#include <QMimeDatabase>

class OdfExtractorTest : public QObject
{
    Q_OBJECT
private:
    QString testFilePath(const QString& fileName) const;

private Q_SLOTS:
    void testText();
    void testTextMetaDataOnly();

    void testPresentation();

    void testTextMissingMetaNoCrash();
    void testTextMissingContentNoCrash();

private:
    QMimeDatabase mimeDb;
};

#endif // ODFEXTRACTORTEST_H
