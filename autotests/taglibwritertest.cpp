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
    QFETCH(QString, stringSuffix);

    QString temporaryFileName = QStringLiteral("writertest.") + fileType;

    QFile::copy(testFilePath("test." + fileType), testFilePath(temporaryFileName));
    TagLibWriter writerPlugin{this};
    QCOMPARE(writerPlugin.writeMimetypes().contains(mimeType),true);

    WriteData data(testFilePath(temporaryFileName), mimeType);

    data.add(Property::Title, QString(QStringLiteral("Title1") + stringSuffix));
    data.add(Property::Artist, QString(QStringLiteral("Artist1") + stringSuffix));
    data.add(Property::Album, QString(QStringLiteral("Album1") + stringSuffix));
    data.add(Property::TrackNumber, 10);
    data.add(Property::ReleaseYear, 1999);
    data.add(Property::Genre, QString(QStringLiteral("Genre1") + stringSuffix));
    data.add(Property::Comment, QString(QStringLiteral("Comment1") + stringSuffix));
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

    QCOMPARE(extractedTitle, QString(QStringLiteral("Title1") + stringSuffix));
    QCOMPARE(extractedArtist, QString(QStringLiteral("Artist1") + stringSuffix));
    QCOMPARE(extractedAlbum, QString(QStringLiteral("Album1") + stringSuffix));
    QCOMPARE(extractedTrackNumber, 10u);
    QCOMPARE(extractedYear, 1999u);
    QCOMPARE(extractedGenre, QString(QStringLiteral("Genre1") + stringSuffix));
    QCOMPARE(extractedComment, QString(QStringLiteral("Comment1") + stringSuffix));

    QFile::remove(testFilePath(temporaryFileName));
}

void TagLibWriterTest::testCommonData_data()
{
    // Add some unicode characters, use codepoints to avoid any issues with
    // source encoding: "€µ"
    static const QChar data[2] = { 0x20ac, 0xb5 };
    QString unicodeTestStringSuffix(data, 2);

    QTest::addColumn<QString>("fileType");
    QTest::addColumn<QString>("mimeType");
    QTest::addColumn<QString>("stringSuffix");


    QTest::addRow("flac")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        << QString()
        ;

    QTest::addRow("flac_unicode")
        << QStringLiteral("flac")
        << QStringLiteral("audio/flac")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("m4a")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        << QString()
        ;

    QTest::addRow("m4a_unicode")
        << QStringLiteral("m4a")
        << QStringLiteral("audio/mp4")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("mp3")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg3")
        << QString()
        ;

    QTest::addRow("mp3_unicode")
        << QStringLiteral("mp3")
        << QStringLiteral("audio/mpeg3")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("mpc")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        << QString()
        ;

    QTest::addRow("mpc_unicode")
        << QStringLiteral("mpc")
        << QStringLiteral("audio/x-musepack")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("ogg")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        << QString()
        ;

    QTest::addRow("ogg_unicode")
        << QStringLiteral("ogg")
        << QStringLiteral("audio/ogg")
        << unicodeTestStringSuffix
        ;

    QTest::addRow("opus")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        << QString()
        ;

    QTest::addRow("opus_unicode")
        << QStringLiteral("opus")
        << QStringLiteral("audio/opus")
        << unicodeTestStringSuffix
        ;
}

QTEST_GUILESS_MAIN(TagLibWriterTest)
