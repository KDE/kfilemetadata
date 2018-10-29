/*
 * Copyright (C) 2017  James D. Smith <smithjd15@gmail.com>
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

#include "usermetadatawritertest.h"
#include "indexerextractortestsconfig.h"
#include "usermetadata.h"

#include <QDebug>
#include <QTest>
#include <QDir>
#include <QFile>

#define TEST_FILENAME "writertest.txt"

using namespace KFileMetaData;

QString UserMetaDataWriterTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QLatin1Char('/') + fileName;
}

void UserMetaDataWriterTest::initTestCase()
{
    QFile testFile(testFilePath("plain_text_file.txt"));
    QFile writerTestFile(testFilePath(TEST_FILENAME));
    QFile::copy(testFilePath("plain_text_file.txt"), testFilePath(TEST_FILENAME));
}

void UserMetaDataWriterTest::test()
{

    KFileMetaData::UserMetaData md(testFilePath(TEST_FILENAME));
    QVERIFY(md.isSupported());

    // Tags
    md.setTags(QStringList() << QStringLiteral("this/is/a/test/tag"));
    QCOMPARE(md.tags().at(0), QStringLiteral("this/is/a/test/tag"));
    md.setTags(QStringList());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.tags")));

    // Rating
    md.setRating(3);
    QCOMPARE(md.rating(), 3);
    md.setRating(0);
    QVERIFY(!md.hasAttribute(QStringLiteral("baloo.rating")));

    // Comment
    md.setUserComment(QStringLiteral("this is a test comment"));
    QCOMPARE(md.userComment(), QStringLiteral("this is a test comment"));
    md.setUserComment(QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.comment")));

    // Origin url
    md.setOriginUrl(QUrl("http://this.is.a.test.website.local"));
    QCOMPARE(md.originUrl(), QUrl("http://this.is.a.test.website.local"));
    md.setOriginUrl(QUrl());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.origin.url")));

    // Origin e-mail subject
    md.setOriginEmailSubject(QStringLiteral("this is a test e-mail subject"));
    QCOMPARE(md.originEmailSubject(), QStringLiteral("this is a test e-mail subject"));
    md.setOriginEmailSubject(QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.origin.email.subject")));

    // Origin e-mail sender
    md.setOriginEmailSender(QStringLiteral("Blue Bear"));
    QCOMPARE(md.originEmailSender(), QStringLiteral("Blue Bear"));
    md.setOriginEmailSender(QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.origin.email.sender")));

    // Origin e-mail message id
    md.setOriginEmailMessageId(QStringLiteral("19991231235959.52234.24C26516HHBTF1C4"));
    QCOMPARE(md.originEmailMessageId(), QStringLiteral("19991231235959.52234.24C26516HHBTF1C4"));
    md.setOriginEmailMessageId(QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.origin.email.message-id")));

    // Attribute
    md.setAttribute(QStringLiteral("test.attribute"), QStringLiteral("attribute"));
    QCOMPARE(md.attribute(QStringLiteral("test.attribute")), QStringLiteral("attribute"));
    md.setAttribute(QStringLiteral("test.attribute"), QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("test.attribute")));
}

void UserMetaDataWriterTest::cleanupTestCase()
{
    QFile::remove(testFilePath(TEST_FILENAME));
}

QTEST_GUILESS_MAIN(UserMetaDataWriterTest)
