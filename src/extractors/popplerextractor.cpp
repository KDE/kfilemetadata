/*
    SPDX-FileCopyrightText: 2012 Vishesh Handa <me@vhanda.in>
    SPDX-FileCopyrightText: 2012 Jörg Ehrichs <joerg.ehrichs@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-or-later
*/


#include "popplerextractor.h"

#include <QScopedPointer>
#include <QDebug>

using namespace KFileMetaData;

PopplerExtractor::PopplerExtractor(QObject* parent)
    : ExtractorPlugin(parent)
{

}

const QStringList supportedMimeTypes = {
    QStringLiteral("application/pdf"),
};

QStringList PopplerExtractor::mimetypes() const
{
    return supportedMimeTypes;
}

void PopplerExtractor::extract(ExtractionResult* result)
{
    const QString fileUrl = result->inputUrl();
    QScopedPointer<Poppler::Document> pdfDoc(Poppler::Document::load(fileUrl, QByteArray(), QByteArray()));

    if (!pdfDoc || pdfDoc->isLocked()) {
        return;
    }

    result->addType(Type::Document);

    if (result->inputFlags() & ExtractionResult::ExtractMetaData) {
        QString title = pdfDoc->info(QStringLiteral("Title")).trimmed();

        if (!title.isEmpty()) {
            result->add(Property::Title, title);
        }

        QString subject = pdfDoc->info(QStringLiteral("Subject"));
        if (!subject.isEmpty()) {
            result->add(Property::Subject, subject);
        }

        QString author = pdfDoc->info(QStringLiteral("Author"));
        if (!author.isEmpty()) {
            result->add(Property::Author, author);
        }

        QString generator = pdfDoc->info(QStringLiteral("Producer"));
        if (!generator.isEmpty()) {
            result->add(Property::Generator, generator);
        }

        QString creationDate = pdfDoc->info(QStringLiteral("CreationDate"));
        if (!creationDate.isEmpty()) {
            QByteArray utf8 = creationDate.toUtf8();
            result->add(Property::CreationDate, Poppler::convertDate(utf8.data()));
        }
    }

    if (!(result->inputFlags() & ExtractionResult::ExtractPlainText)) {
        return;
    }

    for (int i = 0; i < pdfDoc->numPages(); i++) {
        QScopedPointer<Poppler::Page> page(pdfDoc->page(i));
        if (!page) { // broken pdf files do not return a valid page
            qWarning() << "Could not read page content from" << fileUrl;
            break;
        }
        result->append(page->text(QRectF()));
    }
}
