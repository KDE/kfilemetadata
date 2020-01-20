/*
 * This file is part of the KDE KFileMetaData project
 * Copyright (C) 2014  Vishesh Handa <me@vhanda.in>
 *               2017  Igor Poboiko <igor.poboiko@gmail.com>
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
#include <QTest>
#include <QDebug>

#include "extractorcollection.h"

namespace KFileMetaData {

class ExtractorCollectionTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void testFetchExtractors()
    {
        QCoreApplication::setLibraryPaths({QCoreApplication::applicationDirPath()});
        ExtractorCollection collection;
        QVERIFY(collection.fetchExtractors("unknown/mimetype").isEmpty());
        QVERIFY(!collection.fetchExtractors("text/plain").isEmpty());
    }

    void testMultipleExtractorCollections()
    {
        QCoreApplication::setLibraryPaths({QCoreApplication::applicationDirPath()});
        ExtractorCollection collection;
        QVERIFY(collection.fetchExtractors("unknown/mimetype").isEmpty());
        QVERIFY(!collection.fetchExtractors("text/plain").isEmpty());
        ExtractorCollection collection2;
        QVERIFY(collection.fetchExtractors("unknown/mimetype").isEmpty());
        QVERIFY(!collection.fetchExtractors("text/plain").isEmpty());
        QVERIFY(collection2.fetchExtractors("unknown/mimetype").isEmpty());
        QVERIFY(!collection2.fetchExtractors("text/plain").isEmpty());
    }

    void testMimeInheritance()
    {
        QCoreApplication::setLibraryPaths({QCoreApplication::applicationDirPath()});
        ExtractorCollection collection;

        auto textExtractors = collection.fetchExtractors("text/plain");
        QVERIFY(!textExtractors.isEmpty());

        auto xmlExtractors = collection.fetchExtractors("application/xml");
        QVERIFY(!xmlExtractors.isEmpty());

        // Verify the generic "text/plain" extractor is not used for "application/xml"
        for (auto extractor : textExtractors) {
            QVERIFY(!xmlExtractors.contains(extractor));
        }
    }

    void testBestMatching()
    {
        QCoreApplication::setLibraryPaths({QCoreApplication::applicationDirPath()});
        ExtractorCollection collection;

        auto textExtractors = collection.fetchExtractors("text/plain");
        // "application/mathml+xml" is "sub-class-of" "application/xml" (i.e. inherits it),
        // according to the shared-mime-info database
        auto xmlSubtypeExtractors = collection.fetchExtractors("application/mathml+xml");
        QVERIFY(!xmlSubtypeExtractors.isEmpty());

        // Verify the generic "text/plain" extractor is also not used for
        // types inherited from "application/xml"
        for (auto extractor : textExtractors) {
            QVERIFY(!xmlSubtypeExtractors.contains(extractor));
        }
    }

    void testExtractorMetadata()
    {
        QCoreApplication::setLibraryPaths({QCoreApplication::applicationDirPath()});
        ExtractorCollection collection;

        auto allExtractors = collection.allExtractors();

        for (auto extractor : allExtractors) {
            auto exProperties = extractor->extractorProperties();
            if (exProperties.isEmpty()) {
                qWarning() << "Extractor has no property data, please add it! - (Extractor mimetypes:"
                           << extractor->mimetypes().join(", ") + ')';
                continue;
            }

            // Verify properties for every supported mimetype
            auto propMimetypesJson = exProperties["MimeTypes"];
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
            QSet<QString> propMimetypes = propMimetypesJson.toMap().keys().toSet();
            QSet<QString> supportedMimetypes = extractor->mimetypes().toSet();
#else
            const QStringList propMimeList = propMimetypesJson.toMap().keys();
            QSet<QString> propMimetypes(propMimeList.begin(), propMimeList.end());

            const QStringList extractedMimes = extractor->mimetypes();
            QSet<QString> supportedMimetypes(extractedMimes.begin(), extractedMimes.end());
#endif
            QVERIFY2(!exProperties["Name"].toString().isEmpty(), "Missing \"Name\" property");
            QVERIFY2(!exProperties["Id"].toString().isEmpty(),   "Missing \"Id\" property");

            if (propMimetypes == supportedMimetypes) {
                continue;
            }

            auto diff = propMimetypes - supportedMimetypes;
            if (!diff.isEmpty()) {
                for (auto mimetype : diff.values()) {
                    auto mimetypeProp = propMimetypesJson.toMap()[mimetype];
                    auto runtimedep = mimetypeProp.toMap()["HasRuntimeDependency"];
                    if (runtimedep.isValid() && runtimedep.toInt()) {
                        diff.remove(mimetype);
                    }
                }

                if (!diff.isEmpty()) {
                    qWarning() << exProperties["Name"].toString()
                               << exProperties["Id"].toString()
                               << "has extraneous properties for these mimetypes:" << diff.values().join(", ");
                }
            }

            diff = supportedMimetypes - propMimetypes;
            if (!diff.isEmpty()) {
                qWarning() << exProperties["Name"].toString()
                           << exProperties["Id"].toString()
                           << "has no properties for these mimetypes:" + diff.values().join(", ");
            }
        }
    }
};

}

QTEST_GUILESS_MAIN(KFileMetaData::ExtractorCollectionTest)

#include "extractorcollectiontest.moc"
