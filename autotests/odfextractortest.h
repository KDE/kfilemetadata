/*
    SPDX-FileCopyrightText: 2014 Vishesh Handa <me@vhanda.in>
    SPDX-FileCopyrightText: 2016 Christoph Cullmann <cullmann@kde.org>

    SPDX-License-Identifier: LGPL-2.1-or-later
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
    void testNoExtraction();
    void testText();
    void testTextMetaDataOnly();

    void testPresentation();

    void testTextMissingMetaNoCrash();
    void testTextMissingContentNoCrash();

private:
    QMimeDatabase mimeDb;
};

#endif // ODFEXTRACTORTEST_H
