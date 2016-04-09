#include "taglibwritertest.h"
#include "indexerextractortestsconfig.h"
#include "writers/taglibwriter.h"
#include "writedata.h"

#include "taglib.h"
#include "fileref.h"

#include <QDebug>
#include <QTest>
#include <QDir>
#include <QFile>

#define TEST_FILENAME "writertest.opus"

using namespace KFileMetaData;

static QString t2q(const TagLib::String& t)
{
    return QString::fromWCharArray((const wchar_t*)t.toCWString(), t.length());
}

QString TagLibWriterTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void TagLibWriterTest::initTestCase()
{
    QFile testFile(testFilePath("test.opus"));
    QFile writerTestFile(testFilePath(TEST_FILENAME));
    QFile::copy(testFilePath("test.opus"), testFilePath(TEST_FILENAME));
}

void TagLibWriterTest::test()
{
    QScopedPointer<WriterPlugin> writerPlugin(new TagLibWriter(this));

    WriteData data(testFilePath(TEST_FILENAME), "audio/opus");
    data.add(Property::Title, "Title1");
    data.add(Property::Artist, "Artist1");
    writerPlugin->write(data);

    TagLib::FileRef file(testFilePath(TEST_FILENAME).toUtf8().constData(), true);
    TagLib::Tag* tags = file.tag();

    QString extractedTitle = t2q(tags->title());
    QString extractedArtist = t2q(tags->artist());

    QCOMPARE(extractedTitle, QStringLiteral("Title1"));
    QCOMPARE(extractedArtist, QStringLiteral("Artist1"));
}

void TagLibWriterTest::cleanupTestCase()
{
    QFile::remove(testFilePath(TEST_FILENAME));
}

QTEST_MAIN(TagLibWriterTest)
