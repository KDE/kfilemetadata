/*
    SPDX-FileCopyrightText: 2018 Stefan Brüns <stefan.bruens@rwth-aachen.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "postscriptdscextractor.h"
#include "kfilemetadata_debug.h"

#include <QFile>

namespace KFileMetaData
{

DscExtractor::DscExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}

QStringList DscExtractor::mimetypes() const
{
    QStringList list;
    list << QStringLiteral("application/postscript")
         << QStringLiteral("image/x-eps");

    return list;
}

void DscExtractor::extract(ExtractionResult* result)
{
    QFile file(result->inputUrl());
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(KFILEMETADATA_LOG) << "Document is not a valid file";
        return;
    }

    // A little bit heuristic - assume EPS files are images, PS complete documents
    if (result->inputMimetype() == QLatin1String("application/postscript")) {
        result->addType(Type::Document);
    } else {
        result->addType(Type::Image);
    }

    if (!(result->inputFlags() & ExtractionResult::ExtractMetaData)) {
        return;
    }
    // Try to find some DSC (PostScript Language Document Structuring Conventions) conforming data
    QTextStream stream(&file);
    QString line;

    while (stream.readLineInto(&line)) {
        if (!line.startsWith(QLatin1String("%%"))) {
            continue;
        }

        if (const auto tag = QLatin1String("%%Pages:"); line.startsWith(tag)) {
            bool ok = false;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            int pages = QStringView(line).mid(tag.size()).toInt(&ok, 10);
#else
            int pages = line.midRef(tag.size()).toInt(&ok, 10);
#endif
            if (ok) {
                result->add(Property::PageCount, pages);
            }

        } else if (const auto tag = QLatin1String("%%Title:"); line.startsWith(tag)) {
            QStringView title = QStringView(line).mid(tag.size()).trimmed();
            if (title.startsWith(QLatin1Char('(')) && title.endsWith(QLatin1Char(')'))) {
                title = title.mid(1, title.size() - 2);
            }
            if (!title.isEmpty()) {
                result->add(Property::Title, title.toString());
            }

        } else if (const auto tag = QLatin1String("%%CreationDate:"); line.startsWith(tag)) {
            // "Neither the date nor time need be in any standard format."
            QStringView date = QStringView(line).mid(tag.size()).trimmed();
            if (date.startsWith(QLatin1Char('(')) && date.endsWith(QLatin1Char(')'))) {
                date = date.mid(1, date.size() - 2);
                date = date.trimmed();
            }
            if (date.startsWith(QLatin1String("D:")) && date.size() >= 23) {
                // Standard PDF date format, ASN.1 like - (D:YYYYMMDDHHmmSSOHH'mm')
                auto dt = QDateTime::fromString(date.mid(2, 14).toString(), QLatin1String("yyyyMMddhhmmss"));
                auto offset = QTime::fromString(date.mid(17, 5).toString(), QLatin1String("hh'\\''mm"));
                if (date.at(16) == QLatin1Char('+')) {
                    dt.setOffsetFromUtc(QTime(0, 0).secsTo(offset));
                } else {
                    dt.setOffsetFromUtc(-1 * QTime(0, 0).secsTo(offset));
                }
                result->add(Property::CreationDate, dt);
            } else {
                auto dt = QDateTime::fromString(date.toString());
                if (dt.isValid()) {
                    result->add(Property::CreationDate, dt);
                }
            }

        } else if (line.startsWith(QLatin1String("%%EndComments"))) {
            break;
        }
    }
}

} // namespace KFileMetaData
