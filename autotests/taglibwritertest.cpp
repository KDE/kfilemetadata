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

using namespace KFileMetaData;

static QString t2q(const TagLib::String& t)
{
    return QString::fromWCharArray((const wchar_t*)t.toCWString(), t.length());
}

QString TagLibWriterTest::testFilePath(const QString& fileName) const
{
    return QLatin1String(INDEXER_TESTS_SAMPLE_FILES_PATH) + QDir::separator() + fileName;
}

void TagLibWriterTest::testCommonData()
{
    QFETCH(QString, fileType);
    QFETCH(QString, mimeType);

    QString temporaryFileName = QStringLiteral("writertest.") + fileType;

    QFile::copy(testFilePath("test." + fileType), testFilePath(temporaryFileName));
    TagLibWriter writerPlugin{this};
    QCOMPARE(writerPlugin.writeMimetypes().contains(mimeType),true);

    WriteData data(testFilePath(temporaryFileName), mimeType);
    data.add(Property::Title, "Title1");
    data.add(Property::Artist, "Artist1");
    data.add(Property::Album, "Album1");
    data.add(Property::TrackNumber, 10);
    data.add(Property::ReleaseYear, 1999);
    data.add(Property::Genre, "Genre1");
    data.add(Property::Comment, "Comment1");
    writerPlugin.write(data);

    TagLib::FileRef file(testFilePath(temporaryFileName).toUtf8().constData(), true);
    TagLib::Tag* tags = file.tag();

    QString extractedTitle = t2q(tags->title());
    QString extractedArtist = t2q(tags->artist());
    QString extractedAlbum = t2q(tags->album());
    uint extractedTrackNumber = tags->track();
    uint extractedYear = tags->year();
    QString extractedGenre = t2q(tags->genre());
    QString extractedComment = t2q(tags->comment());

    QCOMPARE(extractedTitle, QStringLiteral("Title1"));
    QCOMPARE(extractedArtist, QStringLiteral("Artist1"));
    QCOMPARE(extractedAlbum, QStringLiteral("Album1"));
    QCOMPARE(extractedTrackNumber, 10u);
    QCOMPARE(extractedYear, 1999u);
    QCOMPARE(extractedGenre, QStringLiteral("Genre1"));
    QCOMPARE(extractedComment, QStringLiteral("Comment1"));

    QFile::remove(testFilePath(temporaryFileName));
}

void TagLibWriterTest::testCommonData_data()
{
    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");

    QTest::addRow("flac")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        ;

    QTest::addRow("m4a")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        ;

    QTest::addRow("mp3")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg3")
        ;

    QTest::addRow("mpc")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        ;

    QTest::addRow("ogg")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        ;

    QTest::addRow("opus")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        ;
}

QTEST_GUILESS_MAIN(TagLibWriterTest)
