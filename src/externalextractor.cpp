/*
 * This file is part of the KFileMetaData project
 * Copyright (C) 2016  Varun Joshi <varunj.1011@gmail.com>
 * Copyright (C) 2015  Boudhayan Gupta <bgupta@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "externalextractor.h"
#include "kfilemetadata_debug.h"
#include "properties.h"
#include "propertyinfo.h"
#include "typeinfo.h"

#include <QDir>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define EXTRACTOR_TIMEOUT_MS 30000

using namespace KFileMetaData;

class Q_DECL_HIDDEN ExternalExtractor::ExternalExtractorPrivate
{
public:
    QString path;
    QStringList writeMimetypes;
    QString mainPath;
};


ExternalExtractor::ExternalExtractor(QObject* parent)
    : ExtractorPlugin(parent),
      d_ptr(new ExternalExtractorPrivate)
{
}

ExternalExtractor::ExternalExtractor(const QString& pluginPath)
    : ExtractorPlugin(nullptr),
      d_ptr(new ExternalExtractorPrivate)
{
    Q_D(ExternalExtractor);

    d->path = pluginPath;

    QDir pluginDir(pluginPath);
    QStringList pluginDirContents = pluginDir.entryList(QDir::Files | QDir::NoDotAndDotDot);

    if (!pluginDirContents.contains(QStringLiteral("manifest.json"))) {
        qCDebug(KFILEMETADATA_LOG) << pluginPath << "does not seem to contain a valid plugin";
        return;
    }

    QFile manifest(pluginDir.filePath(QStringLiteral("manifest.json")));
    manifest.open(QIODevice::ReadOnly);
    QJsonDocument manifestDoc = QJsonDocument::fromJson(manifest.readAll());
    if (!manifestDoc.isObject()) {
        qCDebug(KFILEMETADATA_LOG) << "Manifest does not seem to be a valid JSON Object";
        return;
    }

    QJsonObject rootObject = manifestDoc.object();
    const QJsonArray mimetypesArray = rootObject.value(QStringLiteral("mimetypes")).toArray();
    QStringList mimetypes;
    mimetypes.reserve(mimetypesArray.count());

     for (const QVariant &mimetype : mimetypesArray) {
        mimetypes << mimetype.toString();
    }

    d->writeMimetypes.append(mimetypes);
    d->mainPath = pluginDir.filePath(rootObject[QStringLiteral("main")].toString());
}

ExternalExtractor::~ExternalExtractor()
{
    delete d_ptr;
}

QStringList ExternalExtractor::mimetypes() const
{
    Q_D(const ExternalExtractor);

    return d->writeMimetypes;
}

void ExternalExtractor::extract(ExtractionResult* result)
{
    Q_D(ExternalExtractor);

    QJsonDocument writeData;
    QJsonObject writeRootObject;
    QByteArray output;
    QByteArray errorOutput;

    writeRootObject[QStringLiteral("path")] = QJsonValue(result->inputUrl());
    writeRootObject[QStringLiteral("mimetype")] = result->inputMimetype();
    writeData.setObject(writeRootObject);

    QProcess extractorProcess;
    extractorProcess.start(d->mainPath, QStringList(), QIODevice::ReadWrite);
    bool started = extractorProcess.waitForStarted();
    if (!started) {
        qCWarning(KFILEMETADATA_LOG) << "External extractor" << d->mainPath
            << "failed to start:" << extractorProcess.errorString();
        return;
    }

    extractorProcess.write(writeData.toJson());
    extractorProcess.closeWriteChannel();
    extractorProcess.waitForFinished(EXTRACTOR_TIMEOUT_MS);

    output = extractorProcess.readAll();
    errorOutput = extractorProcess.readAllStandardError();

    if (extractorProcess.exitStatus()) {
        qCWarning(KFILEMETADATA_LOG) << "External extractor" << d->mainPath
            << "failed to index" << result->inputUrl() << "-" << errorOutput;
        return;
    }

    // now we read in the output (which is a standard json format) into the
    // ExtractionResult

    QJsonDocument extractorData = QJsonDocument::fromJson(output);
    if (!extractorData.isObject()) {
        return;
    }
    QJsonObject rootObject = extractorData.object();
    QJsonObject propertiesObject = rootObject[QStringLiteral("properties")].toObject();

    const auto propertiesObjectEnd = propertiesObject.constEnd();
    auto i = propertiesObject.constBegin();
    for (; i != propertiesObjectEnd; ++i) {
        if (i.key() == QStringLiteral("typeInfo")) {
            TypeInfo info = TypeInfo::fromName(propertiesObject.value(i.key()).toString());
            result->addType(info.type());
            continue;
        }

        // for plaintext extraction
        if (i.key() == QStringLiteral("text")) {
            result->append(propertiesObject.value(i.key()).toString(QStringLiteral("")));
            continue;
        }

        PropertyInfo info = PropertyInfo::fromName(i.key());
        if (info.name() != i.key()) {
            continue;
        }
        result->add(info.property(), i.value().toVariant());
    }

    if (rootObject[QStringLiteral("status")].toString() != QStringLiteral("OK")) {
        qCDebug(KFILEMETADATA_LOG) << rootObject[QStringLiteral("error")].toString();
    }
}
