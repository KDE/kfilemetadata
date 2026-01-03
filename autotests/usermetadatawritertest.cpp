/*
    SPDX-FileCopyrightText: 2017 James D. Smith <smithjd15@gmail.com>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "indexerextractortestsconfig.h"
#include "usermetadata.h"

#include <QTest>
#include <QFile>

namespace {
const auto TEST_FILENAME = QStringLiteral("writertest-usermetadata.txt");
const auto TEST_SYMLINK = QStringLiteral("dangling_symlink-metadata");

QString testOutputPath(const QString& fileName)
{
    return QStringLiteral(INDEXER_TESTS_OUTPUT_PATH "/UserMetaDataWriterTest/%1").arg(fileName);
}
}

class UserMetaDataWriterTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void initTestCase();
    void test();
    void testMissingPermision();
    void testMetadataSize();
    void testMetadataNameTooLong();
    void testDanglingSymlink();
    void testRemoveMetadata();
    void testMetadataFolder();
    void cleanupTestCase();

private:
    QFile m_writerTestFile;
};

using namespace KFileMetaData;

void UserMetaDataWriterTest::initTestCase()
{
    QVERIFY(QDir().mkpath(testOutputPath({})));

    m_writerTestFile.setFileName(testOutputPath(TEST_FILENAME));
    auto opened = m_writerTestFile.open(QIODevice::WriteOnly | QIODevice::NewOnly);
    QVERIFY(opened);

    QFile::link(testOutputPath(QStringLiteral("invalid_target")), testOutputPath(TEST_SYMLINK));
}

void UserMetaDataWriterTest::testMissingPermision()
{
#ifdef Q_OS_WIN
    QSKIP("Only unix permissions can restrict metadata writing");
#endif
    m_writerTestFile.setPermissions(QFileDevice::ReadOwner);
    KFileMetaData::UserMetaData md(testOutputPath(TEST_FILENAME));
    QVERIFY(md.isSupported());

    auto result = md.setAttribute(QStringLiteral("test"), QStringLiteral("my-value"));
    QCOMPARE(result, KFileMetaData::UserMetaData::MissingPermission);

    QVERIFY(m_writerTestFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner));
}

void UserMetaDataWriterTest::testMetadataSize()
{
    KFileMetaData::UserMetaData md(testOutputPath(TEST_FILENAME));
    QVERIFY(md.isSupported());

    // In the current ext2, ext3, and ext4 filesystem implementations,
    // the total bytes used by the names and values of all of a file's
    // extended attributes must fit in a single filesystem block (1/2/4 kB)

    // all implementations should support at least 512 B
    constexpr auto smallSize = 512; // 512 B
    auto smallValue = QString(smallSize, QLatin1Char('a'));
    auto result = md.setAttribute(QStringLiteral("test"), smallValue);
    QCOMPARE(result, KFileMetaData::UserMetaData::NoError);
    QCOMPARE(md.attribute(QStringLiteral("test")), smallValue);

    // a big value, maxing-out the VFS-imposed limit on Linux
    constexpr auto maxSize = 64 * 1024;
    const auto bigValue = QString(maxSize, QLatin1Char('a')); // 64 kB
    result = md.setAttribute(QStringLiteral("test"), bigValue);
#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_WIN)
    // BSD VFS has no such limit to 64 kB
    QCOMPARE(result, KFileMetaData::UserMetaData::NoError);
    QCOMPARE(md.attribute(QStringLiteral("test")), bigValue);
#else
    if (result == KFileMetaData::UserMetaData::NoSpace) {
        // FS specific limit: nodesize (16 kB) for BTRFS, blocksize on Ext*, ...
        QCOMPARE(result, KFileMetaData::UserMetaData::NoSpace);
    } else {
        // No FS specific limit, full VFS size supported, e.g. XFS
        QCOMPARE(result, KFileMetaData::UserMetaData::NoError);
        QCOMPARE(md.attribute(QStringLiteral("test")), bigValue);
    }
#endif

    // On Linux, the VFS-imposed limits on attribute names and
    // values are 255 bytes and 64 kB, respectively.
    auto excessiveValue = QString(maxSize + 1, QLatin1Char('a'));
    result = md.setAttribute(QStringLiteral("test"), excessiveValue);
#if defined(Q_OS_FREEBSD) || defined(Q_OS_NETBSD) || defined(Q_OS_WIN)
    QCOMPARE(result, KFileMetaData::UserMetaData::NoError);
    QCOMPARE(md.attribute(QStringLiteral("test")), excessiveValue);
#else
    // In Linux, we exceed the max value of an extended attribute, the error is different
    QCOMPARE(result, KFileMetaData::UserMetaData::ValueTooBig);
#endif
}

void UserMetaDataWriterTest::testMetadataNameTooLong()
{
    KFileMetaData::UserMetaData md(testOutputPath(TEST_FILENAME));
    QVERIFY(md.isSupported());

    // BSD and Linux have a limit of the attribute name of 255 bytes
    // Windows has by default a limit on filename that applies to filesystem metadata
    auto longName = QString(256, QLatin1Char('a'));
    int result = md.setAttribute(longName, QStringLiteral("smallValue"));
    QCOMPARE(result, KFileMetaData::UserMetaData::NameToolong);
}

void UserMetaDataWriterTest::test()
{
    auto testFile = testOutputPath(TEST_FILENAME);
    KFileMetaData::UserMetaData md(testFile);
    QVERIFY(md.isSupported());

    // Tags
    const QString tagValue = QStringLiteral("this/is/a/test/tag");
    md.setTags({tagValue});
    QVERIFY(md.hasAttribute(QStringLiteral("xdg.tags")));
    QCOMPARE(md.attribute(QStringLiteral("xdg.tags")), tagValue);
    QCOMPARE(md.tags().isEmpty() ? QStringLiteral("<no tags found>") : md.tags().at(0), QStringLiteral("this/is/a/test/tag"));
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::Any) & UserMetaData::Attribute::Tags);
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::All) & UserMetaData::Attribute::Tags);
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::Tags) & UserMetaData::Attribute::Tags);
    QVERIFY(!(md.queryAttributes(UserMetaData::Attribute::Rating) & UserMetaData::Attribute::Tags));
    md.setTags({});
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.tags")));
    QVERIFY(!(md.queryAttributes(UserMetaData::Attribute::Tags) & UserMetaData::Attribute::Tags));

    // Rating
    md.setRating(3);
    QCOMPARE(md.rating(), 3);
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::All) & UserMetaData::Attribute::Rating);
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::Rating) & UserMetaData::Attribute::Rating);
    md.setRating(0);
    QVERIFY(!md.hasAttribute(QStringLiteral("baloo.rating")));
    QVERIFY(!(md.queryAttributes(UserMetaData::Attribute::All) & UserMetaData::Attribute::Rating));
    QVERIFY(!(md.queryAttributes(UserMetaData::Attribute::Rating) & UserMetaData::Attribute::Rating));

    // Comment
    md.setUserComment(QStringLiteral("this is a test comment"));
    QCOMPARE(md.userComment(), QStringLiteral("this is a test comment"));
    md.setUserComment(QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("xdg.comment")));

    // Origin url
    md.setOriginUrl(QUrl(QStringLiteral("http://this.is.a.test.website.local")));
    QCOMPARE(md.originUrl(), QUrl(QStringLiteral("http://this.is.a.test.website.local")));
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
    md.setAttribute(QStringLiteral("test.attribute2"), QStringLiteral("attribute2"));
    QCOMPARE(md.attribute(QStringLiteral("test.attribute2")), QStringLiteral("attribute2"));
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::All) & UserMetaData::Attribute::Other);
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::Other) & UserMetaData::Attribute::Other);
    md.setAttribute(QStringLiteral("test.attribute"), QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("test.attribute")));
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::All) & UserMetaData::Attribute::Other);
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::Other) & UserMetaData::Attribute::Other);
    md.setAttribute(QStringLiteral("test.attribute2"), QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("test.attribute2")));

    // Check for side effects of calling sequence
    QVERIFY(!md.hasAttribute(QStringLiteral("test.check_contains")));
    md.setAttribute(QStringLiteral("test.check_contains"), QStringLiteral("dummy"));
    QVERIFY(md.hasAttribute(QStringLiteral("test.check_contains")));
    md.setAttribute(QStringLiteral("test.check_contains"), QString());
    QVERIFY(!md.hasAttribute(QStringLiteral("test.check_contains")));
}

void UserMetaDataWriterTest::testDanglingSymlink()
{
    KFileMetaData::UserMetaData md(testOutputPath(TEST_SYMLINK));
    QVERIFY(md.queryAttributes(UserMetaData::Attribute::All) == UserMetaData::Attribute::None);
}

void UserMetaDataWriterTest::testRemoveMetadata()
{
    auto testFile = testOutputPath(TEST_FILENAME);
    KFileMetaData::UserMetaData md(testFile);
    QVERIFY(md.isSupported());

    const auto tagValue = QStringLiteral("this/is/a/test/tag");
    QCOMPARE(md.setAttribute(QStringLiteral("tag"), tagValue), KFileMetaData::UserMetaData::NoError);
    QVERIFY(md.hasAttribute(QStringLiteral("tag")));

    QCOMPARE(md.setAttribute(QStringLiteral("tag"), QString{}), KFileMetaData::UserMetaData::NoError);
    QVERIFY(!md.hasAttribute(QStringLiteral("tag")));
}

void UserMetaDataWriterTest::testMetadataFolder()
{
    const auto dirPath = testOutputPath(QStringLiteral("metadata-dir"));
    QVERIFY(QDir().mkdir(dirPath));

    KFileMetaData::UserMetaData md(dirPath);

    const auto tagValue = QStringLiteral("this/is/a/test/tag");
    QCOMPARE(md.setAttribute(QStringLiteral("tag"), tagValue), KFileMetaData::UserMetaData::NoError);
    QVERIFY(md.hasAttribute(QStringLiteral("tag")));

    QCOMPARE(md.setAttribute(QStringLiteral("tag"), QString{}), KFileMetaData::UserMetaData::NoError);
    QVERIFY(!md.hasAttribute(QStringLiteral("tag")));

    // Removing a non-existing tag is not an error
    QCOMPARE(md.setAttribute(QStringLiteral("tag"), QString{}), KFileMetaData::UserMetaData::NoError);
    QVERIFY(!md.hasAttribute(QStringLiteral("tag")));

    QVERIFY(QDir().rmdir(dirPath));
}

void UserMetaDataWriterTest::cleanupTestCase()
{
    m_writerTestFile.remove();
    QFile::remove(testOutputPath(TEST_SYMLINK));

    QVERIFY(QDir(testOutputPath({})).removeRecursively());
}

QTEST_GUILESS_MAIN(UserMetaDataWriterTest)

#include "usermetadatawritertest.moc"
